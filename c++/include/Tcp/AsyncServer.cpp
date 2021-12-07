//
// Created by Marcos on 04/08/2021.
//

#include "AsyncServer.h"
void AsyncServer::AcceptEvent(AsyncServer *Server,ServerClient* client) {
    ErrorMessage Err;
    Err.code        = ErrorCodes::NoError;
    Err.description = "OK";
    
    std::map<std::string,void*> Args;
    Args["Server"]          = Server;
    Args["Client"]          = client;
    Args["Buffer"]          = NULL;
    Args["ErrorMessage"]    = &Err;
    for(auto event:Server->events[(int)EventTypes::Connected]) {
        event->Main(Args);
    }

    client->Start();
}

void AsyncServer::loopFunction(AsyncServer *Server) {
    while (Server->listening) {

        Server->clients[Server->idCounter] = ServerClient();
        Server->clients[Server->idCounter].setId(Server->idCounter);
        Server->clients[Server->idCounter].setFather(Server);
        Server->clients[Server->idCounter].setEvents(Server->events);

        int clientDataLen = sizeof(Server->clients[Server->idCounter].sockData);

        Server->clients[Server->idCounter]._socket = accept(Server->_socket,(struct sockaddr*)&Server->clients[Server->idCounter].sockData,&clientDataLen);
        
        auto th = std::thread(AsyncServer::AcceptEvent,Server,&Server->clients[Server->idCounter]);
        th.detach();
        Server->idCounter+=1;
    }
}

AsyncServer::AsyncServer(int port, int limit) : port(port), limit(limit) {
    this->events = std::vector<std::vector<MiddlewareModule*>>(10);
}

ErrorMessage AsyncServer::Use(MiddlewareModule* module, EventTypes event, int priority) {
    ErrorMessage DEFAULT;
    DEFAULT.code    = ErrorCodes::NoError;

    module->priority = priority;

    this->events[(int)event].push_back(module);

    std::sort(this->events[(int)event].begin(),this->events[(int)event].end(),[](MiddlewareModule* m1,MiddlewareModule* m2) {
		return (m1->priority < m2->priority);
	});
    return DEFAULT;
}

ErrorMessage AsyncServer::Start(bool detach) {
    this->idCounter     = 0;

    ErrorMessage DEFAULT;
    DEFAULT.code        = ErrorCodes::NoError;
    DEFAULT.description ="OK";

    this->sockData.sin_family       = AF_INET;
    this->sockData.sin_port         = htons(this->port);
    this->sockData.sin_addr.s_addr  = htonl(INADDR_ANY);

    if (bind(this->_socket, (struct sockaddr *) &this->sockData, sizeof(this->sockData)) < 0){
        closesocket(this->_socket);
        DEFAULT.code        = ErrorCodes::BindError;
        DEFAULT.description = "Bind Error";
        return DEFAULT;
    }

    if (listen(this->_socket, this->limit) < 0){
        closesocket(this->_socket);
        DEFAULT.code        = ErrorCodes::ListenError;
        DEFAULT.description = "Listen Error";
        return DEFAULT;
    }

    this->listening     = true;
    this->loopThread    = std::thread(this->loopFunction,this);
    if(detach)
        this->loopThread.detach();
    else
        this->loopThread.join();
    return DEFAULT;
}


//----------------------------------------------------------------------------Client Server

bool ServerClient::isConnected() const {
    return connected;
}

void ServerClient::setConnected(bool connected) {
    ServerClient::connected = connected;
}

int & ServerClient::getId() {
    return id;
}

void ServerClient::setId(int id) {
    ServerClient::id = id;
}

void ServerClient::Start() {
    this->loopThread    = std::thread(this->loopFunction,this);
    this->connected     = true;
    this->loopThread.detach();
}

AsyncServer *ServerClient::getFather() const {
    return father;
}

void ServerClient::setFather(AsyncServer *father) {
    ServerClient::father = father;
}

void ServerClient::loopFunction(ServerClient *Client) {

    ErrorMessage Err;
    Err.code        = ErrorCodes::NoError;
    Err.description = "OK";

    while(Client->connected){
        auto buffer = Buffer(Client->father->DEFAULTBUFFERSIZE);
        buffer.setActualSize(-1);
        buffer.setActualSize(recv(Client->_socket,buffer[0],Client->father->DEFAULTBUFFERSIZE,0));

        if(buffer.getActualSize()<=0) {
            Err.code        = ErrorCodes::MessageError;
            Err.description = "Connection was lost";
            break;
        }

        /**
         * Argumentos a serem passados.
         */
        std::map<std::string,void*> Args;
        Args["Server"]       = Client->father;
        Args["Client"]       = Client;
        Args["Buffer"]       = &buffer;
        Args["ErrorMessage"] = &Err;

        for(auto event:Client->events[(int)EventTypes::Received]){
            event->Main(Args);
        }

    }

    std::map<std::string,void*> Args;
    Args["Server"]          = Client->father;
    Args["Client"]          = Client;
    Args["Buffer"]          = NULL;
    Args["ErrorMessage"]    = &Err;

    for(auto event:Client->events[(int)EventTypes::Disconnected]){
        event->Main(Args);
    }
}

ServerClient::ServerClient() {}

const std::vector<std::vector<MiddlewareModule*>> &ServerClient::getEvents() const {
    return events;
}

void ServerClient::setEvents(std::vector<std::vector<MiddlewareModule*>> &events) {
    ServerClient::events = events;
}

void ServerClient::disconnect() {
    this->connected = false;
    closesocket(this->_socket);
}

ErrorMessage ServerClient::sendBuffer(Buffer data) {
    ErrorMessage Err;
    Err.code        = ErrorCodes::NoError;
    Err.description = "OK";

    std::map<std::string,void*> Args;
    Args["Server"]          = this->father;
    Args["Client"]          = this;
    Args["Buffer"]          = &data;
    Args["ErrorMessage"]    = &Err;

    for(auto event:this->events[(int)EventTypes::Sended]){
        event->Main(Args);
    }

    if(send(this->_socket,data[0],data.getActualSize(),0)!=data.getActualSize()) {
        Err.code        = ErrorCodes::MessageError;
        Err.description = "error sending bytes incompatible amount sent";
        return Err;
    }
    return Err;
}