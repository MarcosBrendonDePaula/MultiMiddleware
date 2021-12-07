//
// Created by Marcos on 24/09/2021.
//

#include "Middleware.h"
#include "../MiddlewareLoader/Tcp/AsyncServer.h"
#include "Node.h"
#include <iostream>
#include <string>

MultiMiddleware* MultiMiddleware::Obj = nullptr;

MultiMiddleware * MultiMiddleware::Make() {
    if(MultiMiddleware::Obj == nullptr)
        MultiMiddleware::Obj = new MultiMiddleware();
    return  MultiMiddleware::Obj;
}

MultiMiddleware::MultiMiddleware() {
}

void MMDisconnect::Main(std::map<std::string, void *> &Args) {
    auto MD = MultiMiddleware::Make();
    auto MN = NodeManipulator::Make();

    ServerClient* client = (ServerClient*)Args["Client"];
    for(auto con:MD->Connections) {
        if(con.second == client) {
            MN->Nodes[con.first].Connected = false;
            break;
        }
    }    
}

void MMConnect::Main(std::map<std::string, void *> &Args) {
    auto MD = MultiMiddleware::Make();
    auto MN = NodeManipulator::Make();

    ServerClient* client = (ServerClient*)Args["Client"];

    Buffer name(1500);
    Buffer lang(1500);

    name.setActualSize(recv(client->_socket,name[0],Config::maxBuffer,0));
    lang.setActualSize(recv(client->_socket,lang[0],Config::maxBuffer,0));
    
    Sleep(600);
    client->sendBuffer(Buffer(Config::Name,Config::maxBuffer));
    Sleep(600);
    client->sendBuffer(Buffer(std::to_string((int)Config::Lang)));

    if(Config::Debug) {
        std::cout<<"Sending:" <<Config::Name<<" Lang:"<< Config::Lang<<std::endl;
    }

    MN->NodeRegister((Langs)std::stoi(lang.toString()),name.toString(),client);
    if(Config::Debug) {
        std::cout<<"Registrando Nome:" <<name.toString()<<" Linguagem:"<< lang.toString()<<std::endl;
    }
}

void MMRecv::Main(std::map<std::string, void *> &Args) {

}

void MMClientConnector::Main(std::map<std::string, void *> &Args) {
    auto MD = MultiMiddleware::Make();
    auto MN = NodeManipulator::Make();
    
    auto client = (AsyncClient*)Args["Client"];

    client->sendBuffer(Buffer(Config::Name));
    Sleep(600);
    client->sendBuffer(Buffer(std::to_string((int)Config::Lang)));
    Sleep(600);

    Buffer name(1500);
    Buffer lang(1500);

    name.setActualSize(recv(client->_socket,name[0],Config::maxBuffer,0));
    lang.setActualSize(recv(client->_socket,lang[0],Config::maxBuffer,0));
    
    MN->NodeRegister((Langs)std::stoi(lang.toString()),name.toString(),client);
    MD->ClientsConnections[name.toString()] = client;
}

ErrorMessage MultiMiddleware::ConnectTo(std::string ip,int port) {
    ErrorMessage Err;
    do{
        AsyncClient *temp = new AsyncClient();
        temp->Use(new MMClientConnector(),EventTypes::Connected,1);
        Err = temp->connect_(ip,port,Config::maxBuffer);
    }while(Err.code == ErrorCodes::ConnectionError);
    return Err;
}