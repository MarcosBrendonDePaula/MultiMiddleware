//
// Created by Marcos on 04/08/2021.
//

#ifndef MIDDLEWARELOADER_SOCKET_H
#define MIDDLEWARELOADER_SOCKET_H

#include <winsock2.h>
#include <cstring>
#include "Buffer.h"
#include "ErrorMessage.h"

class Socket {
public:
    Socket();
    int _socket;
    sockaddr_in sockData;
    /**
     * Windows dll loader
     */
    static WSAData windowsDl;
    static bool windowsSockDlInit;

    virtual Buffer recvBuffer(int maxBuffer = 1500);
    virtual ErrorMessage sendBuffer(Buffer data);
protected:

};


#endif //MIDDLEWARELOADER_SOCKET_H
