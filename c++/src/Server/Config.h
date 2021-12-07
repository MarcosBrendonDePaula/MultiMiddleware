//
// Created by Marcos on 25/09/2021.
//

#ifndef MIDDLEWARELOADER_CONFIG_H
#define MIDDLEWARELOADER_CONFIG_H

#include "Node.h"
#include <iostream>
class Config {
public:
    static int maxBuffer;
    static int CheckConnectionDelay;
    
    static std::string Name;
    static int Lang;

    static bool Debug;
};


#endif //MIDDLEWARELOADER_CONFIG_H
