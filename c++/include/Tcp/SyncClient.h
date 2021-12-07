//
// Created by Marcos on 09/08/2021.
//

#ifndef MIDDLEWARELOADER_SYNCCLIENT_H
#define MIDDLEWARELOADER_SYNCCLIENT_H

#include "./AsyncClient.h"
#include "../ErrorMessage.h"
#include "../Buffer.h"

#include <iostream>


class SyncClient : public AsyncClient{
private:
    bool connected;
public:
    SyncClient();
    ErrorMessage connect_(std::string ip = "127.0.0.1",int port = 25565,int defaultbuffersize = 1500);
    Buffer recvBuffer();

    bool isConnected() const;

    void setConnected(bool connected);
};


#endif //MIDDLEWARELOADER_SYNCCLIENT_H
