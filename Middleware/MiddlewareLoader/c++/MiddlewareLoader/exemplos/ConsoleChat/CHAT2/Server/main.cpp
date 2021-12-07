#include <iostream>
#include <fstream>
#include <map>

#include <Tcp/AsyncServer.h>

std::map<int,ServerClient*> IdToClient;

class OnReceive: public MiddlewareModule{
    void Main(std::map<std::string,void*>& Args) {
        auto client = (ServerClient*)Args["Client"];
        auto buffer = (Buffer*)      Args["Buffer"];

        for (auto cli : IdToClient) {
            if(cli.second->isConnected()) {
                if(cli.second == client){
                    cli.second->sendBuffer(Buffer("<-: "+buffer->toString()));
                }else
                    cli.second->sendBuffer(Buffer("->: "+buffer->toString()));
            }
        }
    }
};

class OnConnect: public MiddlewareModule{
    void Main(std::map<std::string,void*>& Args) {
        auto client = (ServerClient*)Args["Client"];
        IdToClient[client->getId()] = client;
    }
};

class OnDisconnect: public MiddlewareModule{
    void Main(std::map<std::string,void*>& Args) {
        auto client = (ServerClient*)Args["Client"];
        IdToClient.erase(client->getId());
    }
};

int main() {
    AsyncServer server(25565);
    server.Use(new OnReceive(),EventTypes::Received);
    server.Use(new OnConnect(),EventTypes::Connected);
    server.Use(new OnDisconnect(),EventTypes::Disconnected);

    server.Start(false);

    return 0;
}