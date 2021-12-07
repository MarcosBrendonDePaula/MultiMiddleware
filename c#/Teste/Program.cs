using System;
using System.Collections.Generic;
using Middleware;
using MiddlewareLoader;
using MiddlewareLoader.Async;
using Buffer = MiddlewareLoader.Buffer;


namespace Teste
{
    class ResponseTeste : MiddlewareModule
    {
        public override void Main(Dictionary<string, object> args)
        {
            AsyncClientTcp client = (AsyncClientTcp) args["Client"];
            Console.WriteLine("Aguardando variavel..");
            var teste = MultiMiddleware.MultiMiddleware.RecvVariable<double>("nodo1");
            Console.WriteLine(teste);
        }
    }

    class Program
    {
        static void Main(string[] args)
        {
            MultiMiddleware.MultiMiddleware MD = MultiMiddleware.MultiMiddleware.Make();
            NodeManipulator                 NM = NodeManipulator.Make();

            var  s = new MiddlewareLoader.Async.ServerTcp();
            s.Use(new MultiMiddleware.MMConnect(),EventType.Connected);
            s.Start(25566);
            var teste = new AsyncClientTcp();
            teste.Use(new ResponseTeste(), EventType.Received);
            MD.ConnectTo("127.0.0.1", 25565, teste);
            MultiMiddleware.MultiMiddleware.SendVariable<int>(0,"nodo1");
            s.TaskAcceptLoop.Wait();
        }
    }
}
