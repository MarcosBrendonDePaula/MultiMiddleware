using System;
using System.Collections.Generic;
using System.Threading.Tasks;

namespace MiddlewareLoader
{
    public class AsyncClientTcp : Socket
    {
        public Task TaskReciveLoop;
        public List<List<MiddlewareModule>> Events;

        public AsyncClientTcp()
        {
            Events = new List<List<MiddlewareModule>>();
            for (int i = 0; i < 10; i++)
            {
                Events.Add(new List<MiddlewareModule>());
            }

            ErrorMesage Err = new ErrorMesage { code = (int)EventType.ClientStarted, description = "OK" };
            Dictionary<string, Object> args = new Dictionary<string, object>
            {
                {"Client", this}, {"ErrorMessage", Err}
            };

            MiddlewareLoaderConfig.CallEvents(args, Events, EventType.ClientStarted);
        }
         
        public override bool Connect(string ip = "127.0.0.1", int port = 25565, int maxBuffer = 1500)
        {
            var res=base.Connect(ip, port, maxBuffer);

            ErrorMesage Err = new ErrorMesage { code = (int)EventType.Connected, description = "Erro de Conexão" };
            Dictionary<string, Object> args = new Dictionary<string, object>
            {
                {"Client", this}, {"ErrorMessage", Err}
            };
            
            if (!res)
            {
                Err.code = -1;
            }

            MiddlewareLoaderConfig.CallEvents(args, Events, EventType.Connected);
            
            if (!res)
            {
                return false;
            }

            this.TaskReciveLoop = Task.Run(() =>
            {
                RecvLoop(this);
            });
            return Connected;

        }

        public override bool SendBuffer(Buffer buff)
        {
            Dictionary<string, object> args = new Dictionary<string, object>();
            var err = new ErrorMesage { code = (int)EventType.Sended, description = "OK" };

            args.Add("Client", this);
            args.Add("ErrorMessage", err);
            args.Add("Buffer", buff);

            MiddlewareLoaderConfig.CallEvents(args, Events, EventType.Sended);

            return base.SendBuffer(buff); ;
        }

        public void RecvLoop(AsyncClientTcp client)
        {
            Dictionary<string, object> args;
            ErrorMesage err;
            while (client.Connected)
            {
                var novo = client.GetBuffer();
                if (!client.Connected)
                {
                    break;
                }

                args = new Dictionary<string, object>();

                err = new ErrorMesage { code = (int)EventType.Received, description = "OK" };

                args.Add("Client", client);
                args.Add("ErrorMessage", err);
                args.Add("Buffer", novo);
                MiddlewareLoaderConfig.CallEvents(args, Events, EventType.Received);

            }

            args = new Dictionary<string, object>();

            err = new ErrorMesage { code = (int)EventType.Disconnected, description = "Disconnected" };

            args.Add("Client", client);
            args.Add("ErrorMessage", err);

            MiddlewareLoaderConfig.CallEvents(args, Events, EventType.Disconnected);
        }

        public void Use(MiddlewareModule module, EventType ev)
        {
            this.Events[(int)ev].Add(module);
            this.Events[(int)ev].Sort();
        }

    }
}
