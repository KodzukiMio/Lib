using System;
using System.Net;
using System.Net.NetworkInformation;
using System.Net.Sockets;
using System.Numerics;
using System.Runtime.InteropServices;
using System.Security.Cryptography;
using System.Text;

namespace kur {
    using size_t = System.IntPtr;
    static class Mem {
        public unsafe class MemoryPool {//TODO

        }
        public static unsafe void reverse<_Itr>(_Itr* _Begin, _Itr* _End) where _Itr : unmanaged {
            while (_Begin < _End) {
                --_End;
                _Itr _Tmp = *_Begin;
                *_Begin = *_End;
                *_End = _Tmp;
                ++_Begin;
            };
        }
        public static unsafe void* malloc(size_t _Size) {
            return Marshal.AllocHGlobal(_Size).ToPointer();
        }
        public static unsafe void free(void* _Ptr) {
            Marshal.FreeHGlobal((IntPtr)_Ptr);
        }
        public unsafe class Vector<T> where T : unmanaged, IComparable<T> {
            T* chunk = null;
            size_t _size = 0;
            size_t _capacity = 0;
            public void expand() {
                if (chunk == null) {
                    this.create(0x2);
                    return;
                }
                T* tmp = this.chunk;
                size_t esize = sizeof(T) * (_capacity << 1);
                this.chunk = (T*)malloc(esize);
                if (this.chunk == null) {
                    this.chunk = tmp;
                    throw new Exception("bad alloc!");
                }
                Buffer.MemoryCopy(tmp, this.chunk, esize.ToInt64(), _size.ToInt64() * sizeof(T));
                this._capacity = esize >> 1;
                free(tmp);
            }
            public void create(size_t isize) {
                this._capacity = isize;
                this.chunk = (T*)malloc(isize);
            }
            public void construct(size_t isize) {
                if (isize != 0) create(isize);
            }
            public void push_back(T value) {
                if (this._size >= this._capacity) expand();
                *(this.chunk + _size) = value;
                ++this._size;
            }
            public bool is_full() {
                return (this._size + 1) == this._capacity;
            }
            public T* data() {
                return this.chunk;
            }
            public void clear() {
                this._size = 0;
            }
            public ref T at(size_t idx) {
                if (idx >= this._size) throw new Exception("Vector<T> out of range !");
                return ref this.chunk[idx];
            }
            public ref T last() {
                if (this._size == 0) throw new Exception("Vector<T> out of range !");
                return ref this.chunk[this._size - 1];
            }
            public T* begin() {
                return this.chunk;
            }
            public T* end() {
                return this.chunk + this._size;
            }
            public void pop_back() {
                if (this._size == 0) throw new Exception("Vector<T> out of range !");
                --this._size;
            }
            public T* erase(size_t index) {
                if (index >= this._size || this._size == 0) { throw new Exception("Vector<T> out of range !"); };
                size_t _pos = this._size - 1;
                for (size_t i = index; i < _pos; ++i) chunk[i] = chunk[i + 1];
                --_pos;
                return chunk + index;
            }
            public size_t length() {
                return this._size;
            }
            public size_t size() {
                return this._size;
            }
            public size_t capacity() {
                return this._capacity;
            }
            public void init(T val, size_t size = 0) {
                size_t sz = this._size;
                if (size != 0) sz = size;
                for (size_t i = 0; i < sz; ++i) *(this.chunk + i) = val;
            }
            public T* find(ref T value) {
                for (size_t i = 0; i < this._size; ++i) if (chunk[i].Equals(value)) return &chunk[i];
                return end();
            }
            public T this[int i] {
                get => at(i);
                set => chunk[i] = value;
            }
            public Vector(size_t init_size = 0) {
                if (init_size == 0) return;
                this.construct(init_size);
                this._size = init_size;
            }
            ~Vector() {
                if (chunk != null) free(chunk);
            }
        }
    }
    namespace net {
        public static class Method {
            static public async Task Send(NetworkStream io, Encoding enc, string str) {
                try {
                    await io.WriteAsync(BitConverter.GetBytes(str.Length));
                    await io.WriteAsync(enc.GetBytes(str));
                } catch (IOException) {
                    io.Close();
                }
            }
            static public async Task<string?> Receive(NetworkStream io, Encoding enc) {
                try {
                    byte[] buf4 = new byte[4];
                    int readlen = 0;
                    while (readlen < 4) {
                        int read = await io.ReadAsync(buf4, readlen, 4 - readlen);
                        if (read == 0) {
                            io.Close();
                            return null;
                        }
                        readlen += read;
                    }
                    int msglen = BitConverter.ToInt32(buf4, 0);
                    byte[] buffer = new byte[msglen];
                    readlen = 0;
                    while (readlen < msglen) {
                        int read = await io.ReadAsync(buffer, readlen, msglen - readlen);
                        if (read == 0) {
                            io.Close();
                            return null;
                        }
                        readlen += read;
                    }
                    return enc.GetString(buffer);
                } catch (IOException) {
                    io.Close();
                };
                return null;
            }
        }
        public class Server {
            public TcpListener listener;
            public IPAddress ip_adress;
            public List<Task>? client_tasks = null;
            public int port = 0;
            public bool continue_ = true;
            public Encoding encoding = Encoding.ASCII;
            public delegate Task<bool> Request(NetworkStream io);
            Request request;
            public Server(ref string ip, int port, Request req) {
                if (ip.Length == 0) this.ip_adress = IPAddress.Any;
                else this.ip_adress = IPAddress.Parse(ip);
                this.port = port;
                this.listener = new TcpListener(this.ip_adress, this.port);
                this.request = req;
            }
            public void SetEncoding(Encoding enc) {
                this.encoding = enc;
            }
            public void Start() {
                listener.Start();
                client_tasks = new List<Task>();
                while (continue_) {
                    client_tasks.Add(Handle(listener.AcceptTcpClient()));
                    client_tasks.RemoveAll(t => t.IsCompleted || t.IsFaulted || t.IsCanceled);
                };
            }
            public void Stop() {
                this.continue_ = false;
                listener.Stop();
            }
            public async Task Handle(TcpClient client) {
                using NetworkStream iostr = client.GetStream();
                try {
                    while (await request(iostr)) ;
                } finally {
                    iostr.Close();
                    client.Close();
                }
            }
            ~Server() {
                this.Stop();
            }
        }
        public class Client {
            public TcpClient client;
            public IPAddress ip_adress;
            public int port;
            public Encoding encoding = Encoding.ASCII;
            public Client(ref string ip, int port, Encoding enc) {
                this.ip_adress = IPAddress.Parse(ip);
                this.port = port;
                this.client = new TcpClient();
                this.encoding = enc;
            }
            public void Connect() {
                client.Connect(this.ip_adress, this.port);
            }
            public void Close() {
                client.Close();
            }
            public NetworkStream GetStream() {
                return client.GetStream();
            }
            public async void SendAsync(string str) {
                await Method.Send(client.GetStream(), this.encoding, str);
            }
            public async Task<string?> ReceiveAsync() {
                string? str = await Method.Receive(client.GetStream(), this.encoding);
                return str;
            }
            ~Client() {
                client.Close();
            }
        }
        public class HttpServer {
            public int port;
            HttpListener listener;
            public bool continue_ = true;
            public bool outlog = false;
            HttpListenerContext? content = null;
            public delegate bool Request(HttpListenerContext req);
            Request request;
            public HttpServer(int port, Request req) {
                this.port = port;
                this.listener = new HttpListener();
                this.listener.Prefixes.Add($"http://+:{port}/");
                this.request = req;
            }
            public void Start() {
                listener.Start();
            step:
                if (!NetworkInterface.GetIsNetworkAvailable()) {
                    Console.WriteLine("No network connection available.");
                    Thread.Sleep(1000);
                    goto step;
                };
                try {
                    while (continue_ && request(this.content = listener.GetContext())) ;
                } catch (Exception ex) {
                    Console.WriteLine(ex.ToString());
                    if (this.content != null) {
                        this.content.Response.Close();
                        //this.Stop();
                        if (outlog) {
                            System.Diagnostics.Trace.WriteLine($"{DateTime.Now}:{ex}");
                            System.Diagnostics.Trace.Flush();
                        }
                    }
                }
                goto step;
            }
            public void Stop() {
                this.continue_ = false;
                listener.Stop();
            }
        }
    }
}
