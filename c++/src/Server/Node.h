//
// Created by Marcos on 24/09/2021.
//

#ifndef MIDDLEWARELOADER_NODE_H
#define MIDDLEWARELOADER_NODE_H

#include <iostream>
#include <Tcp/AsyncServer.h>
#include "Config.h"
#include <map>

enum class Langs{
    Csharp,
    Cpp,
};

class Node {
public:
    Langs         Lang;
    bool          Connected;
    Socket*       Client;
    std::string   Name;
};

class NodeManipulator {
public:
    static std::map<std::string,Node> Nodes;
    static NodeManipulator* Obj;
    static NodeManipulator* Make();

    NodeManipulator();
    void NodeRegister(Langs Lang,std::string Name,Socket* Client);
};

#endif //MIDDLEWARELOADER_NODE_H
