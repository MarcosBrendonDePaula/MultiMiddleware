//
// Created by Marcos on 25/09/2021.
//

#include "Config.h"
//Middleware Config
bool Config::Debug               = true;

//Node Information
std::string Config::Name         = "nodo1";
int Config::Lang                 = (int)Langs::Cpp;
int Config::maxBuffer            = 1500;
int Config::CheckConnectionDelay = 100;
