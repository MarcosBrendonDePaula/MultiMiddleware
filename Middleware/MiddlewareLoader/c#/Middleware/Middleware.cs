using System;
using System.Collections.Generic;
using System.IO;
using System.Net.Sockets;
using System.Runtime.Serialization.Formatters.Binary;
using System.Threading;
using Middleware;
using MiddlewareLoader;
using MiddlewareLoader.Async;
using Buffer = MiddlewareLoader.Buffer;
using Socket = MiddlewareLoader.Socket;

namespace MultiMiddleware
{
    public class MMConnect : MiddlewareModule
    {
        public MMConnect()
        {
            this.Priority = 0;
        }
        public override void Main(Dictionary<string, object> Args)
        {
            Console.WriteLine("Conectou!");
            var client = (ServerClientTcp) Args["Client"];
            var MD = MultiMiddleware.Make();
            var MN = NodeManipulator.Make();

            var Name = client.GetBuffer();
            var Lang = client.GetBuffer();

            Thread.Sleep(600);
            client.SendBuffer(new Buffer(Config.Name));
            Thread.Sleep(600);
            client.SendBuffer(new Buffer(Convert.ToString((int)Config.Lang)));

            if (Config.Debug)
            {
                Console.WriteLine("Sending:"+Config.Name + " Lang:" + Config.Lang);
            }

            MN.NodeRegister((Config.Langs)Convert.ToInt32(Lang.ToString()), Name.ToString(), client);

            if (Config.Debug)
            {
                Console.WriteLine("add," + Lang.ToString() + ","+ Name.ToString());
            }
        }
    }
    public class MMClientConnector:MiddlewareModule
    {
        public override void Main(Dictionary<string, object> args)
        {
            var MD = MultiMiddleware.Make();
            var MN = NodeManipulator.Make();

            var client = (AsyncClientTcp)args["Client"];

            client.SendBuffer(new Buffer(Config.Name));
            Thread.Sleep(600);
            client.SendBuffer(new Buffer(Convert.ToString((int) Config.Lang)));
            Thread.Sleep(600);

            string name;
            Config.Langs lang;

            name = client.GetBuffer().ToString();

            lang = (Config.Langs) Int32.Parse(client.GetBuffer().ToString());
            MN.NodeRegister(lang, name, client);
        }
    }
    public class MultiMiddleware
    {
        private static byte[] ObjectToByteArray<type_>(type_ obj)
        {
            if (typeof(type_) == typeof(bool))
                return BitConverter.GetBytes((bool)Convert.ChangeType(obj, typeof(bool)));
            if (typeof(type_) == typeof(int))
                return BitConverter.GetBytes((int)Convert.ChangeType(obj,typeof(int)));
            if(typeof(type_) == typeof(float)) 
                return BitConverter.GetBytes((float)Convert.ChangeType(obj, typeof(float)));
            if(typeof(type_) == typeof(double))
                return BitConverter.GetBytes((double)Convert.ChangeType(obj, typeof(double)));
            if (typeof(type_) == typeof(char))
                return BitConverter.GetBytes((char)Convert.ChangeType(obj, typeof(char)));
            if (typeof(type_) == typeof(long))
                return BitConverter.GetBytes((long)Convert.ChangeType(obj, typeof(long)));
            if (typeof(type_) == typeof(uint))
                return BitConverter.GetBytes((uint)Convert.ChangeType(obj, typeof(uint)));
            if (typeof(type_) == typeof(ulong))
                return BitConverter.GetBytes((ulong)Convert.ChangeType(obj, typeof(ulong)));
            if (typeof(type_) == typeof(short))
                return BitConverter.GetBytes((short)Convert.ChangeType(obj, typeof(short)));
            if (typeof(type_) == typeof(ushort))
                return BitConverter.GetBytes((ushort)Convert.ChangeType(obj, typeof(ushort)));
            return new byte[1];

        }

        private static type_ ByteArrayToObject<type_>(byte[] obj)
        {
            
            if (typeof(type_) == typeof(bool))
                return (type_)Convert.ChangeType(BitConverter.ToBoolean(obj, 0), typeof(type_));
            if (typeof(type_) == typeof(int))
                return (type_) Convert.ChangeType(BitConverter.ToInt32(obj, 0), typeof(type_));
            if (typeof(type_) == typeof(double))
                return (type_)Convert.ChangeType(BitConverter.ToDouble(obj, 0), typeof(type_));
            if (typeof(type_) == typeof(char))
                return (type_)Convert.ChangeType(BitConverter.ToChar(obj, 0), typeof(type_));
            if (typeof(type_) == typeof(long))
                return (type_)Convert.ChangeType(BitConverter.ToInt64(obj, 0), typeof(type_));
            if (typeof(type_) == typeof(uint))
                return (type_)Convert.ChangeType(BitConverter.ToUInt32(obj, 0), typeof(type_));
            if (typeof(type_) == typeof(ulong))
                return (type_)Convert.ChangeType(BitConverter.ToUInt64(obj, 0), typeof(type_));
            if (typeof(type_) == typeof(short))
                return (type_)Convert.ChangeType(BitConverter.ToInt16(obj, 0), typeof(type_));
            if (typeof(type_) == typeof(ushort))
                return (type_)Convert.ChangeType(BitConverter.ToUInt16(obj, 0), typeof(type_));
            return (type_)Convert.ChangeType(BitConverter.ToUInt16(obj, 0), typeof(type_));
        }

        public Dictionary<string, AsyncClientTcp> Connections;
        public Dictionary<string, AsyncClientTcp> ClientsConnections;

        public static MultiMiddleware OBJ = null;

        public static MultiMiddleware Make()
        {
            if (MultiMiddleware.OBJ == null)
                MultiMiddleware.OBJ = new MultiMiddleware();
            return MultiMiddleware.OBJ;
        }
        public static void SendToCpp<type_>(type_ obj, string alvo)
        {
            if (Config.Debug)
            {
                Console.WriteLine("Sending variable:"+ obj.GetType().Name + "to node in language cpp node: " + alvo);
            }
            Socket Client = NodeManipulator.Nodes[alvo].Client;
            var array = ObjectToByteArray<type_>(obj);
            Client.Socket_.Send(array,array.Length,SocketFlags.None);
        }
        public static void SendToCSharp<type_>(type_ obj, string alvo)
        {
            if (Config.Debug)
            {
                Console.WriteLine("Sending variable:" + obj.GetType().Name + "to node in language cpp node: " + alvo);
            }
            Socket Client = NodeManipulator.Nodes[alvo].Client;
            var array = ObjectToByteArray<type_>(obj);
            Client.Socket_.Send(array, array.Length, SocketFlags.None);
        }

        public static type_ RecvToCpp<type_>(string alvo) where type_ : new()
        {
            if (Config.Debug)
            {
            }
            Socket client = NodeManipulator.Nodes[alvo].Client;

            var bff= client.GetBuffer();

            type_ obj = ByteArrayToObject<type_>(bff.Data);
            return obj;
        }

        public static type_ RecvToCsharp<type_>(string alvo) where type_ : new()
        {
            if (Config.Debug)
            {
            }
            Socket Client = NodeManipulator.Nodes[alvo].Client;

            int bytes = 0;
            type_ temp = new type_();

            long size;
            using (Stream s = new MemoryStream())
            {
                BinaryFormatter formatter = new BinaryFormatter();
                formatter.Serialize(s, new type_());
                size = s.Length;
            }
            byte[] array = new byte[size];
            bytes = Client.Socket_.Receive(array);

            MemoryStream memStream = new MemoryStream();
            BinaryFormatter binForm = new BinaryFormatter();
            memStream.Write(array, 0, array.Length);
            memStream.Seek(0, SeekOrigin.Begin);
            type_ obj = (type_)binForm.Deserialize(memStream);
            return obj;
        }

        public static void SendVariable<type_>(type_ obj,string alvo)
        {
            while (!NodeManipulator.Nodes[alvo].Connected)
            {
                if (Config.Debug)
                {
                    Console.WriteLine("waiting connection of "+ alvo);
                }
                Thread.Sleep(Config.CheckConnectionDelay);
            }
            switch (NodeManipulator.Nodes[alvo].Lang)
            {
                case Config.Langs.csharp:
                {
                    MultiMiddleware.SendToCSharp<type_>(obj, alvo);
                    break;
                }

                case Config.Langs.cpp:
                {
                    MultiMiddleware.SendToCpp<type_>(obj, alvo);
                    break;
                }
            }
        }

        public static type_ RecvVariable<type_>(string alvo) where type_ : new()
        {
            while (!NodeManipulator.Nodes[alvo].Connected)
            {
                if (Config.Debug)
                {
                    Console.WriteLine("waiting connection of " + alvo);
                }
                Thread.Sleep(Config.CheckConnectionDelay);
            }
            switch (NodeManipulator.Nodes[alvo].Lang)
            {
                case Config.Langs.csharp:
                {
                    return MultiMiddleware.RecvToCsharp<type_>(alvo);
                }

                case Config.Langs.cpp:
                {
                    return MultiMiddleware.RecvToCpp<type_>(alvo);
                }
            }
            return new type_();
        }

        public MultiMiddleware()
        {

        }

        public bool ConnectTo(string ip,int port, AsyncClientTcp temp)
        {
            bool connected = false;
            if (temp == null)
            {
                temp = new AsyncClientTcp();
            }

            do
            {
                temp.Use(new MMClientConnector(), EventType.Connected);
                connected = temp.Connect(ip, port, Config.MaxBuffer);
                if (!connected)
                    temp = null;
            } while (!connected);
            return connected;
        }
    }
}
