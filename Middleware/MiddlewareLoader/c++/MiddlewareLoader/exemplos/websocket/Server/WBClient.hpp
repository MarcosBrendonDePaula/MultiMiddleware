#pragma once
#include <iostream>
#include <fstream>
#include <map>

#include <Tcp/AsyncServer.h>
#include "ModuloHttp/websocket/modulo.hpp"

using namespace std;
using json = nlohmann::json;

class WBClient : public MiddlewareModule{
    private:
        bool answered;
    public:
        void Main(map<string,void *>&Args) {
            auto client = (ServerClient*)Args["Client"];
            if((Args["answered"] != nullptr)){
                cout<<"WBClient: algum modulo respondeu primeiro"<<endl;
                return;
            }

            if(Args["wbclient"] != nullptr) {
                auto buffer = (Buffer*)Args["Buffer"]; 
                auto instance = (WebsocketInstance*)Args["instance"];
                
                auto resp = instance->DecodeDataGrama(*buffer);
                
                if(resp.first < 0) {
                    Args["answered"] = &answered;
                    instance->connected = false;
                    WebSocket::Instances.erase(client);
                    client->disconnect();
                    return;
                }

                auto temp = resp.second + " Funcionou!";
                instance->SendDataGram(temp);
                Args["answered"] = &answered;
            }
        }
};