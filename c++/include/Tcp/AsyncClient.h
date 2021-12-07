//
// Created by Marcos on 07/08/2021.
//

#ifndef MIDDLEWARELOADER_ASYNCCLIENT_H
#define MIDDLEWARELOADER_ASYNCCLIENT_H

#include "../Socket.h"
#include "../MiddlewareModule.h"
#include "../ErrorMessage.h"
#include "../EventTypes.h"
#include "../Buffer.h"

#include <thread>
#include <map>
#include <vector>
#include <algorithm>

class AsyncClient : public Socket{

private:
    bool connected;
    std::string ip;
    int port;
    int DEFAULTBUFFERSIZE=1500;
    std::vector<std::vector<MiddlewareModule*>> events;

    std::thread loopThread;
    static void loopFunction(AsyncClient* Client);
public:
    AsyncClient();

    ErrorMessage Use(MiddlewareModule* module,EventTypes event,int priority = 5);

    bool isConnected() const;

    const std::string &getIp() const;

    void setIp(const std::string &ip);

    int getPort() const;

    void setPort(int port);

    const std::vector<std::vector<MiddlewareModule*>> &getEvents() const;

    void setEvents(const std::vector<std::vector<MiddlewareModule*>> &events);

    const std::thread &getLoopThread() const;

    ErrorMessage connect_(std::string ip = "127.0.0.1",int port = 25565,int defaultbuffersize = 1500,bool detach = true);

    int getDefaultbuffersize() const;

    void setDefaultbuffersize(int defaultbuffersize);

    ErrorMessage sendBuffer(Buffer data);

    void disconnect();
};


#endif //MIDDLEWARELOADER_ASYNCCLIENT_H
