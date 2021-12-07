//
// Created by Marcos on 04/08/2021.
//

#ifndef MIDDLEWARELOADER_ERRORMESSAGE_H
#define MIDDLEWARELOADER_ERRORMESSAGE_H
#include <iostream>

enum ErrorCodes: int {
    NoError,
    BindError,
    ListenError,
    ConnectionError,
    MessageError,
};

class ErrorMessage {
public:
    std::string description;
    int code;
};


#endif //MIDDLEWARELOADER_ERRORMESSAGE_H
