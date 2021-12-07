//
// Created by Marcos on 07/08/2021.
//

#include "AsyncClient.h"

AsyncClient::AsyncClient() {
    this->connected = false;
    this->events = std::vector<std::vector<MiddlewareModule*>>(10);
}

bool AsyncClient::isConnected() const {
    return connected;
}

const std::string &AsyncClient::getIp() const {
    return ip;
}

void AsyncClient::setIp(const std::string &ip) {
    AsyncClient::ip = ip;
}

int AsyncClient::getPort() const {
    return port;
}

void AsyncClient::setPort(int port) {
    AsyncClient::port = port;
}

const std::vector<std::vector<MiddlewareModule*>> &AsyncClient::getEvents() const {
    return events;
}

void AsyncClient::setEvents(const std::vector<std::vector<MiddlewareModule*>> &events) {
   this->events = events;
}

const std::thread &AsyncClient::getLoopThread() const {
    return loopThread;
}

ErrorMessage AsyncClient::connect_(std::string ip, int port,int defaultbuffersize,bool detach) {

    this->setIp(ip);
    this->setPort(port);
    this->setDefaultbuffersize(defaultbuffersize);

    this->sockData.sin_family       = AF_INET;
    this->sockData.sin_addr.s_addr  = inet_addr(ip.c_str());
    this->sockData.sin_port         = htons(port);
    ErrorMessage Err;
    Err.code        = ErrorCodes::NoError;
    Err.description = "OK";

    if(connect(this->_socket,(struct sockaddr*)&this->sockData,sizeof(this->sockData)) == -1) {
        Err.code        = ErrorCodes::ConnectionError;
        Err.description = "Check IP or port";
        return Err;
    }
	
	this->connected = true;
	
    std::map<std::string,void*> Args;
    Args["Client"]          = this;
    Args["Buffer"]          = NULL;
    Args["ErrorMessage"]    = &Err;

    for(auto event:this->events[(int)EventTypes::Connected]){
        event->Main(Args);
    }
    
    if(detach) {
		this->loopThread = std::thread(AsyncClient::loopFunction,this);
        this->loopThread.detach();
    }
    else {
        this->loopThread.join();
    }
    return Err;
}

int AsyncClient::getDefaultbuffersize() const {
    return DEFAULTBUFFERSIZE;
}

void AsyncClient::setDefaultbuffersize(int defaultbuffersize) {
    DEFAULTBUFFERSIZE = defaultbuffersize;
}

ErrorMessage AsyncClient::Use(MiddlewareModule* module, EventTypes event, int priority) {
    ErrorMessage DEFAULT;
    DEFAULT.code    = ErrorCodes::NoError;

    module->priority = priority;

    this->events[(int)event].push_back(module);

    std::sort(this->events[(int)event].begin(),this->events[(int)event].end());
    return DEFAULT;
}

ErrorMessage AsyncClient::sendBuffer(Buffer data) {
    ErrorMessage Err;
    Err.code        = ErrorCodes::NoError;
    Err.description = "OK";

    std::map<std::string,void*> Args;
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

void AsyncClient::disconnect() {
    this->connected = false;
    closesocket(this->_socket);
}

void AsyncClient::loopFunction(AsyncClient *Client) {

    ErrorMessage Err;
    Err.code        = ErrorCodes::NoError;
    Err.description = "OK";

    while(Client->connected){
        auto buffer = Buffer(Client->DEFAULTBUFFERSIZE);
        buffer.setActualSize(-1);
        buffer.setActualSize(recv(Client->_socket,buffer[0],Client->DEFAULTBUFFERSIZE,0));

        if(buffer.getActualSize()<=0) {
            Err.code        = ErrorCodes::MessageError;
            Err.description = "Connection was lost";
            break;
        }

        /**
         * Argumentos a serem passados.
         */
        std::map<std::string,void*> Args;
        Args["Client"]       = Client;
        Args["Buffer"]       = &buffer;
        Args["ErrorMessage"] = &Err;

        for(auto event:Client->events[(int)EventTypes::Received]){
            event->Main(Args);
        }

    }

    std::map<std::string,void*> Args;
    Args["Client"]          = Client;
    Args["Buffer"]          = NULL;
    Args["ErrorMessage"]    = &Err;

    for(auto event:Client->events[(int)EventTypes::Disconnected]){
        event->Main(Args);
    }
}
