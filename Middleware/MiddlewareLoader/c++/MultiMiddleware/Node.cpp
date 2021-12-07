//
// Created by Marcos on 24/09/2021.
//

#include "Node.h"
std::map<std::string,Node> NodeManipulator::Nodes = std::map<std::string,Node>();
NodeManipulator* NodeManipulator::Obj             = nullptr;

NodeManipulator * NodeManipulator::Make() {
    if(NodeManipulator::Obj == nullptr) {
        NodeManipulator::Obj = new NodeManipulator();
    }
    return NodeManipulator::Obj;
}

NodeManipulator::NodeManipulator() {

}

void NodeManipulator::NodeRegister(Langs Lang,std::string Name,Socket* Client) {
    auto novoNode = Node();

    novoNode.Lang   = Lang;
    novoNode.Name   = Name;
    novoNode.Client = Client;
    novoNode.Connected = true;
    
    this->Nodes[Name] = novoNode;
}

