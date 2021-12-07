#include <iostream>
#include <Tcp/AsyncServer.h>
#include <MiddlewareModule.h>
#include "Middleware.h"
#include "Node.h"

class TesteResponse:public MiddlewareModule {
    public:
    void Main(std::map<std::string,void*>& args) {
        auto client = (ServerClient*)args["Client"];
        client->sendBuffer(Buffer("T"));
        MultiMiddleware::SendVariable<double>(30.89,"csnodo");
        std::cout<<"Enviando variavel"<<std::endl;
    }
};

int main() {
    MultiMiddleware* MD = MultiMiddleware::Make();
    NodeManipulator* NM = NodeManipulator::Make();

    AsyncServer server(25565,-1);
    server.Use(new MMConnect(),EventTypes::Connected,5);
    server.Use(new TesteResponse(),EventTypes::Received,5);
    server.Start(true);

    MD->ConnectTo("127.0.0.1",25566);
    while(true) {
        Sleep(1000);
    }
    return 0;
}