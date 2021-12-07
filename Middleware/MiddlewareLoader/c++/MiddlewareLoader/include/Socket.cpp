//
// Created by Marcos on 04/08/2021.
//

#include "Socket.h"
WSAData Socket::windowsDl;
bool Socket::windowsSockDlInit = false;

/**
 * Caso o sistema operaçional seja windows a biblioteca referente ao gerenciamento do _socket será carregada
 */
Socket::Socket() {
    if(!Socket::windowsSockDlInit) {
        if(WSAStartup(MAKEWORD(2,2),&Socket::windowsDl) == 0) {
            Socket::windowsSockDlInit = true;
        }
    }
    std::memset(&this->sockData,0x0,sizeof(this->sockData));
    this->_socket = socket(AF_INET, SOCK_STREAM, 0);
}

Buffer Socket::recvBuffer(int maxBuffer) {
    auto buffer = Buffer(maxBuffer);
    buffer.setActualSize(-1);
    buffer.setActualSize(recv(this->_socket,buffer[0],maxBuffer,0));
    return buffer;
}

ErrorMessage Socket::sendBuffer(Buffer data) {
    ErrorMessage Err;
    Err.code        = ErrorCodes::NoError;
    Err.description = "OK";

    if(send(this->_socket,data[0],data.getActualSize(),0)!=data.getActualSize()) {
        Err.code        = ErrorCodes::MessageError;
        Err.description = "error sending bytes incompatible amount sent";
        return Err;
    }
    return Err;
}