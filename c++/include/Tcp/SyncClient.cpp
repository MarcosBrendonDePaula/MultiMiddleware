//
// Created by Marcos on 09/08/2021.
//

#include "SyncClient.h"

SyncClient::SyncClient() {
    this->connected = false;
}

ErrorMessage SyncClient::connect_(std::string ip, int port,int defaultbuffersize) {

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
    return Err;
}

Buffer SyncClient::recvBuffer() {
    auto buffer = Buffer(this->getDefaultbuffersize());
    buffer.setActualSize(-1);
    buffer.setActualSize(recv(this->_socket,buffer[0],this->getDefaultbuffersize(),0));
    return buffer;
}

bool SyncClient::isConnected() const {
    return connected;
}

void SyncClient::setConnected(bool connected) {
    SyncClient::connected = connected;
}
