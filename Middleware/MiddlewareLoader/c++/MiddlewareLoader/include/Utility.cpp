//
// Created by Marcos on 09/08/2021.
//

#include "Utility.h"

template<class varType> void Utility::sendVariable(varType var, Socket target) {
    send(target._socket,&var,sizeof(var),0);
}

template<class varType> varType Utility::recvVariable(Socket target) {
    varType var;
    recv(target._socket,&var,sizeof(var),0);
    return var;
}

