//
// Created by Marcos on 09/08/2021.
//

#ifndef MIDDLEWARELOADER_UTILITY_H
#define MIDDLEWARELOADER_UTILITY_H
#include "./Socket.h"

class Utility {
public:
    template<class varType> static void sendVariable(varType var, Socket target);
    template<class varType> static varType recvVariable(Socket target);

};


#endif //MIDDLEWARELOADER_UTILITY_H
