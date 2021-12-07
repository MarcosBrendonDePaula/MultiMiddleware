//
// Created by Marcos on 28/09/2021.
//

#ifndef MIDDLEWARELOADER_FUNCTIONMANAGER_H
#define MIDDLEWARELOADER_FUNCTIONMANAGER_H

#include "./Middleware.h"
#include "./Node.h"
#include <map>
class FunctionManager {
public:
    static FunctionManager* obj;
    static FunctionManager* Make();
    FunctionManager();
};


#endif //MIDDLEWARELOADER_FUNCTIONMANAGER_H
