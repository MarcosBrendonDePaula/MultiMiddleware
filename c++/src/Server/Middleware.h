//
// Created by Marcos on 24/09/2021.
//

#ifndef MIDDLEWARELOADER_MIDDLEWARE_H
#define MIDDLEWARELOADER_MIDDLEWARE_H

#include <MiddlewareModule.h>
#include <Tcp/AsyncServer.h>
#include <Tcp/AsyncClient.h>
#include "./Config.h"

class MMRecv: public MiddlewareModule{
    public:
    void Main(std::map<std::string,void*>& Args);
};

class MMConnect: public MiddlewareModule {
    public:
    void Main(std::map<std::string,void*>& Args);
};

class MMDisconnect: public MiddlewareModule {
    void Main(std::map<std::string,void*>& Args);
};

class MMClientConnector: public MiddlewareModule {
    void Main(std::map<std::string,void*>& Args);
};

class MultiMiddleware{
public:
    static MultiMiddleware* Obj;
    static MultiMiddleware* Make();
    
    template<typename type_> 
    static void SendToCpp(type_ obj,std::string alvo){
        if(Config::Debug){
            std::cout<<"Sending variable:"<<typeid(type_).name()<<" to node in language cpp node:"<<alvo<<std::endl;
        }
        Socket* Client = NodeManipulator::Nodes[alvo].Client;
        send(Client->_socket,(char*)&obj,sizeof(obj),0);
    }
    
    template<typename type_> 
    static void SendToCsharp(type_ obj,std::string alvo){
        if(Config::Debug){
            std::cout<<"Sending variable: "<<typeid(type_).name()<<" to node in language csharp node:"<<alvo<<std::endl;
        }
        Socket* Client = NodeManipulator::Nodes[alvo].Client;
        send(Client->_socket,(char*)&obj,sizeof(obj),0);
    }

    template<typename type_> 
    static type_ RecvToCsharp(std::string alvo){
        if(Config::Debug){
            std::cout<<"recv variable: "<<typeid(type_).name()<<" to node in language csharp node:"<<alvo<<std::endl;
        }
        Socket* Client = NodeManipulator::Nodes[alvo].Client;
        int bytes = 0;
        type_ temp;
        bytes = recv(Client->_socket,(char*)&temp,sizeof(temp),0);
        return temp;
    }
    
    template<typename type_> 
    static type_ RecvToCpp(std::string alvo){
        if(Config::Debug){
            std::cout<<"reciving variable:"<<typeid(type_).name()<<" to node in language cpp node:"<<alvo<<std::endl;
        }
        Socket* Client = NodeManipulator::Nodes[alvo].Client;
        int bytes = 0;
        type_ temp;
        bytes = recv(Client->_socket,(char*)&temp,sizeof(temp),0);
        return temp;
    }

    template<typename type> 
    static void SendVariable(type obj,std::string alvo) {
        while(!NodeManipulator::Nodes[alvo].Connected) {
            if(Config::Debug){
                    std::cout<<"waiting connection of "<<alvo<<std::endl;
                }
                Sleep(Config::CheckConnectionDelay);
            }
            switch (NodeManipulator::Nodes[alvo].Lang) {
                case Langs::Csharp :{
                    MultiMiddleware::SendToCsharp<type>(obj,alvo);
                    break;
                }

                case Langs::Cpp :{
                    MultiMiddleware::SendToCpp<type>(obj,alvo);
                    break;
                }
        }
    }
    
    template<typename type> 
    static type RecvVariable(std::string alvo) {
        while(!NodeManipulator::Nodes[alvo].Connected) {
            if(Config::Debug){
                    std::cout<<"waiting connection of "<<alvo<<std::endl;
                }
                Sleep(Config::CheckConnectionDelay);
            }
            switch (NodeManipulator::Nodes[alvo].Lang) {
                case Langs::Csharp :{
                    return MultiMiddleware::RecvToCsharp<type>(alvo);
                    break;
                }

                case Langs::Cpp :{
                    return MultiMiddleware::RecvToCpp<type>(alvo);
                    break;
                }
        }
    }

    std::map<std::string,ServerClient*> Connections;
    std::map<std::string,AsyncClient*> ClientsConnections;
    
    MultiMiddleware();
    ErrorMessage ConnectTo(std::string ip,int port);
};


#endif //MIDDLEWARELOADER_MIDDLEWARE_H
