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

namespace KUR {
    public delegate dynamic ActionOrFunc(dynamic args);
    public delegate TResult ActionOrFunc<TResult>(params object[] args);
    /*ActionOrFunc myFunc = (args) =>
{
    foreach (var arg in args) {
        ...
    }
    return ...
};
*/
    public class V8 : BaseClass<V8> {
        public bool input_flag = true;
        public V8ScriptEngine engine;
        public ScriptC scriptC;
        public bool net = false;
        public V8() {
            this.engine = new V8ScriptEngine();
            this.scriptC = new ScriptC(this.engine, this);
            Load();
        }
        public void Load() {
            engine.AddHostObject("SYS", this.scriptC);
            engine.AddHostType(typeof(JsonConvert));
            engine.AddHostObject("LIB", new Microsoft.ClearScript.HostTypeCollection("mscorlib", "System.Core"));
            engine.Execute(@"const Cout =function (value) {if (typeof value === 'function') {value = value.toString(); } else {value = SYS.SerializeObject(value);}SYS.WriteLine(value);};");
            engine.Execute(@"const _ = (obj) => {Cout(typeof(obj));let properties = new Set();let currentObj = obj;do {Object.getOwnPropertyNames(currentObj).map(item => properties.add(item));} while ((currentObj = Object.getPrototypeOf(currentObj)))return [...properties.keys()];}");
            engine.Execute(@"const Console=LIB.System.Console;console.log=function(v){Console.WriteLine(v);};");
            engine.Execute(@"let LOGTMP=[];console.log=function(v){LOGTMP.push(v);};function _OUT(){let str='';for(let i =0;i<LOGTMP.length;i++){str+=LOGTMP[i];str+='<br>';};LOGTMP=[];return str;}");
        }
        static string _tmp = @"_OUT()";
        public string GetOutPut() {//net
            return _Eval(ref _tmp).Replace("\"", "");
        }
        public void EnableNetwork() {
            this.net = true;
        }
        public void PrintDynamicNames() {
            dynamic dynamicResult = engine.Evaluate("this");
            foreach (var name in dynamicResult.GetDynamicMemberNames())
                Console.WriteLine("{0}: {1}", name, dynamicResult[name]);
        }
        public void SetInputFlag(bool _flag) { this.input_flag = _flag; }
        public string Eval(ref string str) {//net or local
            string ret = _Eval(ref str);
            if (!this.net) return ret;
            string retc = GetOutPut();
            return retc.Length > 0 ? retc : ret;
        }
        public string _Eval(ref string str) {//local
            try {
                var result = engine.Evaluate(str);
                if (result != null) {
                    if (result.ToString() == "[undefined]") return ("");
                    var serializedResult = JsonConvert.SerializeObject(result);
                    return serializedResult;
                }
            } catch (Exception) {
                try {
                    return JsonConvert.SerializeObject(engine.Evaluate("_(" + str + ");"));
                } catch (Exception ex_) {
                    return ex_.Message;
                }
            }
            return ("");
        }
        public void InterActive(string Fmsg = ">>") {
            string? input;
            input_flag = true;
            while (input_flag) {
                Console.Write(Fmsg);
                input = Console.ReadLine();
                if (input != null) Console.WriteLine(_Eval(ref input));
            };
        }

    };
    public class ScriptC {
        public V8ScriptEngine engine;
        public V8 v8;
        public ScriptC(V8ScriptEngine _engine, V8 v8_) {
            this.engine = _engine;
            this.v8 = v8_;
        }
        public string SerializeObject(object value) {
            try {
                return JsonConvert.SerializeObject(value);
            } catch (Exception ex) {
                return ex.Message;
            };
        }
        public void Global() {
            var propertyNames = engine.Global.PropertyNames.GetEnumerator();
            while (propertyNames.MoveNext()) {
                string element = propertyNames.Current;
                string gettype = ("(typeof " + element + ")");
                Console.WriteLine($"{v8.Eval(ref gettype)} :'{element}'");
            }
            propertyNames.Dispose();
        }
        public dynamic Window() { return engine.Script; }
        public void WriteLine(string value) {
            Console.WriteLine(value);
        }
        public void EXIT() { v8.SetInputFlag(false); }
    }
    public abstract class BaseClass<T> where T : BaseClass<T>, new() {//静态创建
        public static T Create(params object[] args) {
            return (T)Activator.CreateInstance(typeof(T), args);
        }
        public static T Create() {
            return new T();
        }
    };
    class Fst {
        public static Action<string> Cout = System.Console.WriteLine;
        static public void PressContinue(string msg) { Fst.Cout(msg); Console.ReadKey(); }
        public static bool SendEmail(string reciveAddress, string Subject, string body, string from = "Kurzer@foxmail.com", string key = "umpolxknqzdrbadi", string client = "smtp.qq.com", int port = 587, bool ssl = true) {
            try {
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
            } catch (Exception e) {
                Console.WriteLine(e.ToString());
                return false;
            }
        }
    }

    public static class JSON {
        public static dynamic? ToObj<T>(string json) { return json != null ? Newtonsoft.Json.JsonConvert.DeserializeObject<T>(json) : null; }
        public static string ToJson(dynamic obj) { return Newtonsoft.Json.JsonConvert.SerializeObject(obj); }
        public static bool LoadAndCreateConfig<T>(ref T obj, string path) {
            if (System.IO.File.Exists(path)) {
                obj = KUR.JSON.ToObj<T>(System.IO.File.ReadAllText(path));
                return true;
            } else {
                if (obj != null) System.IO.File.WriteAllText(path, KUR.JSON.ToJson(obj));
                return false;
            };
        }
    }
    public static class Encrypt {
        static public string CalculateMD5Hash(string input) {
            using (MD5 md5 = MD5.Create()) {
                byte[] inputBytes = Encoding.UTF8.GetBytes(input);
                StringBuilder sb = new StringBuilder();
                foreach (byte b in md5.ComputeHash(inputBytes)) {
                    sb.Append(b.ToString("x2"));
                }
                return sb.ToString();
            }
        }
    }
}

