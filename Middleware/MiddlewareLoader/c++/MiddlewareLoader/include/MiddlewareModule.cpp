//
// Created by Marcos on 04/08/2021.
//

#include "MiddlewareModule.h"

bool MiddlewareModule::operator<(MiddlewareModule* obj) {
    return this->priority<obj->priority;
}
