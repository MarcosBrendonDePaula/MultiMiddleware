#include <iostream>
#include <fstream>
#include <map>

#include <Tcp/AsyncServer.h>

std::map<int,std::string> IdToRoom;
std::map<std::string,std::map<int,ServerClient*>> Room;

class OnReceive: public MiddlewareModule{
    void Main(std::map<std::string,void*>& Args) {
        auto client = (ServerClient*)Args["Client"];
        auto buffer = (Buffer*)      Args["Buffer"];

        if(buffer->toString().find("room,") == 0){
            std::string room = buffer->toString();
            if(Room[IdToRoom[client->getId()]][client->getId()] != nullptr){
                Room[IdToRoom[client->getId()]].erase(client->getId());
            }
            IdToRoom[client->getId()] = room;
            Room[room][client->getId()] = client;
        }
        else
        for (auto cli : Room[IdToRoom[client->getId()]]) {
            if(cli.second->isConnected()) {
                if(cli.second == client){
                    cli.second->sendBuffer(Buffer(IdToRoom[client->getId()]+" <-: "+buffer->toString()));
                }else
                    cli.second->sendBuffer(Buffer(IdToRoom[client->getId()]+" ->: "+buffer->toString()));
            }
        }
    }
};

class OnConnect: public MiddlewareModule{
    void Main(std::map<std::string,void*>& Args) {
        auto client = (ServerClient*)Args["Client"];
        IdToRoom[client->getId()] = "Default";
        Room[IdToRoom[client->getId()]][client->getId()] = client;
    }
};

class OnDisconnect: public MiddlewareModule{
    void Main(std::map<std::string,void*>& Args) {
        auto client = (ServerClient*)Args["Client"];
        Room[IdToRoom[client->getId()]].erase(client->getId());
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