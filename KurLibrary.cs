using System;
using Microsoft.ClearScript.V8;
using Newtonsoft.Json;
using System.Collections.Generic;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;
using System.Linq;
using System.Net.Mail;
using System.IO;
using System.Threading;
using System.Web;
using System.Security.Cryptography;

namespace KUR
{
    public class V8 : BaseClass<V8>
    {
        public bool input_flag = true;
        public V8ScriptEngine engine = null;
        public ScriptC scriptC = null;
        public V8()
        {
            this.engine = new V8ScriptEngine();
            this.scriptC = new ScriptC(this.engine, this);
            Load();
        }
        public void Load()
        {
            engine.AddHostObject("SYS", this.scriptC);
            engine.AddHostType(typeof(JsonConvert));
            engine.AddHostObject("LIB", new Microsoft.ClearScript.HostTypeCollection("mscorlib", "System.Core"));
            engine.Execute(@"const Cout =function (value) {if (typeof value === 'function') {value = value.toString(); } else {value = SYS.SerializeObject(value);}SYS.WriteLine(value);};");
            engine.Execute(@"const _ = (obj) => {Cout(typeof(obj));let properties = new Set();let currentObj = obj;do {Object.getOwnPropertyNames(currentObj).map(item => properties.add(item));} while ((currentObj = Object.getPrototypeOf(currentObj)))return [...properties.keys()];}");
            engine.Execute(@"const Console=LIB.System.Console;");
            dynamic dynamicResult = engine.Evaluate("this");
            foreach (var name in dynamicResult.GetDynamicMemberNames())
                Console.WriteLine("{0}: {1}", name, dynamicResult[name]);
        }
        public void SetInputFlag(bool _flag) { this.input_flag = _flag; }
        public string Eval(ref string str)
        {
            try
            {
                var result = engine.Evaluate(str);
                if (result != null)
                {
                    if (result.ToString() == "[undefined]")
                    {
                        Console.WriteLine("[undefined]");
                        return ("");
                    };
                    var serializedResult = JsonConvert.SerializeObject(result);
                    return serializedResult;
                }
            } catch (Exception)
            {
                try
                {
                    Console.WriteLine(JsonConvert.SerializeObject(engine.Evaluate("_(" + str + ");")));
                } catch (Exception ex_)
                {
                    return ex_.Message;
                }
            }
            return ("____________");
        }
        public void InterActive(string Fmsg = ">>")
        {
            string input;
            input_flag = true;
            while (input_flag)
            {
                Console.Write(Fmsg);
                input = Console.ReadLine();
                Console.WriteLine(Eval(ref input));
            };
        }

    };
    public class ScriptC
    {
        public V8ScriptEngine engine = null;
        public V8 v8 = null;
        public ScriptC(V8ScriptEngine _engine, V8 v8_)
        {
            this.engine = _engine;
            this.v8 = v8_;
        }
        public string SerializeObject(object value)
        {
            try
            {
                return JsonConvert.SerializeObject(value);
            } catch (Exception ex)
            {
                return ex.Message;
            };
        }
        public void Global()
        {
            var propertyNames = engine.Global.PropertyNames.GetEnumerator();
            while (propertyNames.MoveNext())
            {
                string element = propertyNames.Current;
                string gettype = ("(typeof " + element + ")");
                Console.WriteLine($"{v8.Eval(ref gettype)} :'{element}'");
            }
            propertyNames.Dispose();
        }
        public dynamic Window() { return engine.Script; }
        public void WriteLine(string value)
        {
            Console.WriteLine(value);
        }
        public void EXIT() { v8.SetInputFlag(false); }
    }
    public abstract class BaseClass<T> where T : BaseClass<T>, new()
    {//静态创建
        public static T Create(params object[] args)
        {
            return (T)Activator.CreateInstance(typeof(T), args);
        }
        public static T Create()
        {
            return new T();
        }
    };
    class Fst
    {
        public static Action<string> Cout = System.Console.WriteLine;
        static public void PressContinue(string msg) { Fst.Cout(msg); Console.ReadKey(); }
        public static bool SendEmail(string reciveAddress, string Subject, string body, string from = "Kurzer@foxmail.com", string key = "umpolxknqzdrbadi", string client = "smtp.qq.com", int port = 587, bool ssl = true)
        {
            try
            {
                MailMessage mail = new MailMessage();
                mail.From = new MailAddress(from);
                mail.To.Add(reciveAddress);
                mail.Subject = Subject;
                mail.Body = body;
                SmtpClient smtp = new SmtpClient(client);
                smtp.Port = port;
                smtp.Credentials = new System.Net.NetworkCredential(from, key);
                smtp.EnableSsl = ssl;
                smtp.Send(mail);
                return true;
            } catch (Exception e)
            {
                Console.WriteLine(e.ToString());
                return false;
            }
        }
    }

    delegate dynamic ActionOrFunc(dynamic args);
    delegate TResult ActionOrFunc<TResult>(params object[] args);
    /*ActionOrFunc myFunc = (args) =>
    {
        foreach (var arg in args) {
            ...
        }
        return ...
    };

    // 调用方法
    string result = (string)myFunc(123, "abc", DateTime.Now);
    Console.WriteLine(result);*/
    public delegate void SocketHandle(StreamWriter writer_, StreamReader reader_, Server server);


    public class Server
    {
        private TcpListener server;
        private Boolean isRunning;
        public SocketHandle sh;
        public dynamic outSource;
        public Server(int port, KUR.SocketHandle sh_)
        {
            server = new TcpListener(IPAddress.Any, port);
            server.Start();
            isRunning = true;
            this.sh = sh_;
        }

        public void StartLoopClients()
        {
            while (isRunning)
            {
                TcpClient newClient = server.AcceptTcpClient();

                Thread t = new Thread(new ParameterizedThreadStart(HandleClient));
                t.Start(newClient);
            }
        }

        public void HandleClient(object obj)
        {
            TcpClient client = (TcpClient)obj;

            StreamWriter sWriter = new StreamWriter(client.GetStream(), Encoding.UTF8);
            StreamReader sReader = new StreamReader(client.GetStream(), Encoding.UTF8);

            Boolean bClientConnected = true;

            while (bClientConnected)
            {
                try
                {
                    if (sReader.BaseStream.CanRead) // 检查StreamReader是否已经关闭
                    {
                        sh(sWriter, sReader, this);
                    } else
                    {
                        bClientConnected = false;
                    }
                } catch (IOException)
                {
                    bClientConnected = false;
                    sWriter.Close();
                    sReader.Close();
                    client.Close();
                } catch (ObjectDisposedException)
                {
                    bClientConnected = false;
                    sWriter.Close();
                    sReader.Close();
                    client.Close();
                }
            }
        }

        public void Send(StreamWriter sWriter, string data)
        {
            sWriter.WriteLine(data);
            sWriter.Flush();
        }

        public string Receive(StreamReader sReader)
        {
            string message = sReader.ReadLine();
            return message;
        }
    }
    public class Client
    {
        private TcpClient client;
        private StreamWriter sWriter;
        private StreamReader sReader;
        public Client(string ipAddress, int port)
        {
            client = new TcpClient();
            client.Connect(ipAddress, port);
            sWriter = new StreamWriter(client.GetStream(), Encoding.UTF8);
            sReader = new StreamReader(client.GetStream(), Encoding.UTF8);
        }

        public void Send(string data)
        {
            try
            {
                sWriter.WriteLine(data);
                sWriter.Flush();
            } catch (IOException)
            {
                sWriter.Close();
                sReader.Close();
                client.Close();
            }
        }

        public string Receive()
        {
            string message = null;
            try
            {
                message = sReader.ReadLine();
            } catch (IOException)
            {
                sWriter.Close();
                sReader.Close();
                client.Close();
            }
            return message;
        }
    }
    public static class JSON
    {
        public static dynamic ToObj<T>(string json) { return Newtonsoft.Json.JsonConvert.DeserializeObject<T>(json); }
        public static string ToJson(dynamic obj) { return Newtonsoft.Json.JsonConvert.SerializeObject(obj); }
        public static bool LoadAndCreateConfig<T>(ref T obj, string path)
        {
            if (System.IO.File.Exists(path))
            {
                obj = KUR.JSON.ToObj<T>(System.IO.File.ReadAllText(path));
                return true;
            } else
            {
                System.IO.File.WriteAllText(path, KUR.JSON.ToJson(obj));
                return false;
            };
        }
    }
    public static class Encrypt
    {
        static public string CalculateMD5Hash(string input)
        {
            using (MD5 md5 = MD5.Create())
            {
                // 将字符串转换为字节数组
                byte[] inputBytes = Encoding.UTF8.GetBytes(input);

                // 计算MD5哈希值并将结果转换为十六进制字符串
                StringBuilder sb = new StringBuilder();
                foreach (byte b in md5.ComputeHash(inputBytes))
                {
                    sb.Append(b.ToString("x2"));
                }
                return sb.ToString();
            }
        }
    }
    public class AesEncryption
    {
        private byte[] Key;
        private byte[] IV;

        public AesEncryption()
        {
            using (Aes myAes = Aes.Create())
            {
                Key = myAes.Key;
                IV = myAes.IV;
            }
        }
        public void SetKI(byte[] key, byte[] iv) { this.Key = key; this.IV = iv; }

        public string Encrypt(string plainText)
        {
            byte[] encrypted;

            using (Aes aesAlg = Aes.Create())
            {
                aesAlg.Key = Key;
                aesAlg.IV = IV;

                ICryptoTransform encryptor = aesAlg.CreateEncryptor(aesAlg.Key, aesAlg.IV);

                using (MemoryStream msEncrypt = new MemoryStream())
                {
                    using (CryptoStream csEncrypt = new CryptoStream(msEncrypt, encryptor, CryptoStreamMode.Write))
                    {
                        using (StreamWriter swEncrypt = new StreamWriter(csEncrypt))
                        {
                            swEncrypt.Write(plainText);
                        }
                        encrypted = msEncrypt.ToArray();
                    }
                }
            }

            return Convert.ToBase64String(encrypted);
        }

        public string Decrypt(string cipherText)
        {
            string plaintext = null;
            byte[] cipherBytes = Convert.FromBase64String(cipherText);

            using (Aes aesAlg = Aes.Create())
            {
                aesAlg.Key = Key;
                aesAlg.IV = IV;

                ICryptoTransform decryptor = aesAlg.CreateDecryptor(aesAlg.Key, aesAlg.IV);

                using (MemoryStream msDecrypt = new MemoryStream(cipherBytes))
                {
                    using (CryptoStream csDecrypt = new CryptoStream(msDecrypt, decryptor, CryptoStreamMode.Read))
                    {
                        using (StreamReader srDecrypt = new StreamReader(csDecrypt))
                        {
                            plaintext = srDecrypt.ReadToEnd();
                        }
                    }
                }
            }

            return plaintext;
        }
    }

}

