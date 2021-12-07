using System.Collections.Generic;
using System.Threading.Tasks;

namespace MiddlewareLoader
{

    namespace Async
    {
        public class ServerClientTcp : Socket
        {
            /// <summary>
            /// le o Servidor
            /// </summary>
            public ServerTcp Servidor { get; }
            
            /// <summary>
            /// le a Id do cliente
            /// </summary>
            public int Id { get; }

            /// <summary>
            /// thread do loop de recebimento.
            /// </summary>
            public Task TsakRecvLoop { get; set; }

            /// <summary>
            /// Lista de eventos a serem executadas.
            /// </summary>
            public List<List<MiddlewareModule>> Events;
            
            /// <summary>
            /// metodo construtor do cliente
            /// </summary>
            /// <param name="id"></param>
            /// <param name="sv"></param>
            public ServerClientTcp(int id,ServerTcp sv)
            {
                this.Id = id;
                this.Servidor = sv;
            }
            
            /// <summary>
            /// Envia o Buff ao alvo.
            /// </summary>
            /// <param name="buff"></param>
            /// <returns></returns>
            public override bool SendBuffer(Buffer buff)
            {
                Dictionary<string, object> args = new Dictionary<string, object>();
                var err = new ErrorMesage { code = (int)EventType.Sended, description = "OK" };

                args.Add("Server", this.Servidor);
                args.Add("Client", this);
                args.Add("ErrorMessage", err);
                args.Add("Buffer", buff);

                MiddlewareLoaderConfig.CallEvents(args, Events, EventType.Sended);

                return base.SendBuffer(buff); ;
            }

            /// <summary>
            /// Inicia a thread de comunicação entre os pontos
            /// </summary>
            public void InitCommunication()
            {
                this.TsakRecvLoop = Task.Run(() =>
                {
                    RecvLoop(this);
                });
            }


            /// <summary>
            /// Função asincrona para ficar recebendo mensagens.
            /// </summary>
            /// <param name="client"></param>
            private void RecvLoop(ServerClientTcp client)
            {
                Dictionary<string, object> args;
                ErrorMesage err;
                while (client.Connected)
                {
                    var novo = client.GetBuffer(client.MaxBuffer);
                    if (!client.Connected)
                    {
                        break;
                    }

                    args = new Dictionary<string, object>();

                    err = new ErrorMesage {code = (int) EventType.Received, description = "OK"};

                    args.Add("Server", client.Servidor);
                    args.Add("Client", client);
                    args.Add("ErrorMessage", err);
                    args.Add("Buffer", novo);
                    MiddlewareLoaderConfig.CallEvents(args, Events, EventType.Received);
                   
                }

                args = new Dictionary<string, object>();

                err = new ErrorMesage {code = (int) EventType.Disconnected, description = "Disconnected"};

                args.Add("Server", client.Servidor);
                args.Add("Client", client);
                args.Add("ErrorMessage", err);

                MiddlewareLoaderConfig.CallEvents(args, Events, EventType.Disconnected);

                client.Servidor.Clients.Remove(client.Id);

            }
        }
    }
}
