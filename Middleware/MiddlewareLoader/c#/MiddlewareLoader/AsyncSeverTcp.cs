using System;
using System.Collections.Generic;
using System.Threading.Tasks;

namespace MiddlewareLoader
{
    namespace Async
    {
        public class ServerTcp : Socket
        {
            public Task TaskAcceptLoop;

            public Dictionary<int, ServerClientTcp> Clients;
            public List<List<MiddlewareModule>> Events;

            private int IdCounter { get; set; }

            public ServerTcp()
            {
                IdCounter = 0;
                Clients = new Dictionary<int, ServerClientTcp>();
                Events = new List<List<MiddlewareModule>>();
                for (int i = 0; i < 10; i++)
                {
                    Events.Add(new List<MiddlewareModule>());
                }
            }

            public void Start(int port = 25565, int maxBuffer = 1500, int backLog = 10)
            {

                ErrorMesage Err = new ErrorMesage { code = (int)EventType.ServerStarted, description = "OK" };

                this.MaxBuffer = maxBuffer;
                this.Port      = port;
                if (!this.Listen("127.0.0.1", port, backLog))
                {
                    Err.description = "Erro ao colocar socket em modo listen";
                    Err.code *= -1;
                }

                Dictionary<string, Object> args = new Dictionary<string, object>
                {
                    {"Server", this}, {"ErrorMessage", Err}
                };

                MiddlewareLoaderConfig.CallEvents(args, Events, EventType.ServerStarted);

                this.TaskAcceptLoop = Task.Run(() =>
                {
                    AcceptLoop(this);
                });
            }

            public void Use(MiddlewareModule module, EventType ev)
            {
                this.Events[(int)ev].Add(module);
                this.Events[(int)ev].Sort();
            }

            private void AcceptLoop(ServerTcp Server)
            {
                while (Server.Listening)
                {
                    ServerClientTcp client = new ServerClientTcp(Server.IdCounter, this)
                    {
                        Socket_ = Server.AcceptConnection(),
                        Events = Server.Events,
                        MaxBuffer = Server.MaxBuffer
                    };

                    Dictionary<string, Object> args = new Dictionary<string, object>();

                    ErrorMesage Err = new ErrorMesage {code = (int) EventType.Connected, description = "OK"};

                    args.Add("Server", this);
                    args.Add("Client", client);
                    args.Add("ErrorMessage", Err);

                    MiddlewareLoaderConfig.CallEvents(args, Server.Events, EventType.Connected);

                    Clients.Add(Server.IdCounter,client);
                    client.InitCommunication();
                    Server.IdCounter      += 1;
                    
                }

            }
        }
    }
}
