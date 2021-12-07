//
// Created by Marcos on 04/08/2021.
//

#ifndef MIDDLEWARELOADER_ASYNCSERVER_H
#define MIDDLEWARELOADER_ASYNCSERVER_H

#include "../Socket.h"
#include "../EventTypes.h"
#include "../ErrorMessage.h"
#include "../Buffer.h"
#include "../MiddlewareModule.h"

#include <vector>
#include <map>
#include <iostream>
#include <algorithm>
#include <thread>

class ServerClient;
class AsyncServer : public Socket {
private:
    int port;
    int limit;
    bool listening;
    unsigned int idCounter;
    std::vector<std::vector<MiddlewareModule*>> events;
    std::map<unsigned int,ServerClient> clients;

    std::thread loopThread;
    static void loopFunction(AsyncServer* Server);
    static void AcceptEvent(AsyncServer *Server,ServerClient* client);
public:
    int DEFAULTBUFFERSIZE = 1500;
    AsyncServer(int port = 25565, int limit = -1);
    ErrorMessage Use(MiddlewareModule* module,EventTypes event,int priority = 5);
    ErrorMessage Start(bool detach = true);
};


class ServerClient : public Socket{
private:
    bool connected;
    int id;
    AsyncServer* father;
    std::vector<std::vector<MiddlewareModule*>> events;

    std::thread loopThread;
    static void loopFunction(ServerClient* Client);

public:

    void disconnect();

    ServerClient();

    void Start();

    AsyncServer *getFather() const;

    void setFather(AsyncServer *father);

    bool isConnected() const;

    void setConnected(bool connected);

    int & getId();

    void setId(int id);

    const std::vector<std::vector<MiddlewareModule*>> &getEvents() const;

    void setEvents(std::vector<std::vector<MiddlewareModule*>> &events);

    ErrorMessage sendBuffer(Buffer data);
};

#endif //MIDDLEWARELOADER_ASYNCSERVER_H
