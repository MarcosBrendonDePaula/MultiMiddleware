#include <iostream>
#include <fstream>
#include <map>

#include <Tcp/AsyncServer.h>
#include "./ModuloHttp/modulo.hpp"

int main() {
    AsyncServer server(25565);

    server.Use(new HttpServer(),EventTypes::Received);
    server.Use(new RemoveInstances(),EventTypes::Disconnected);
    server.Start(false);

    return 0;
}