//
// Created by Marcos on 28/09/2021.
//

#include "FunctionManager.h"
FunctionManager* FunctionManager::obj = nullptr;

FunctionManager * FunctionManager::Make() {
    if(FunctionManager::obj == nullptr)
        FunctionManager::obj = new FunctionManager();
    return FunctionManager::obj;
}

FunctionManager::FunctionManager() {

}