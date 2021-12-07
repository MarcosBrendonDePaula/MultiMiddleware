//
// Created by Marcos on 04/08/2021.
//

#ifndef MIDDLEWARELOADER_BUFFER_H
#define MIDDLEWARELOADER_BUFFER_H

#include <iostream>
#include <cstring>
#include <vector>

class Buffer {
private:
    std::vector<char> data;
    int maxSize;
    int actualSize;
public:
    Buffer(){}
    Buffer(int maxSize = 1500);
    Buffer(std::string str = "",int maxSize = 1500);

    const std::vector<char> &getData() const;

    void setData(const std::vector<char> &data);

    int getMaxSize() const;

    void setMaxSize(int maxSize);

    int getActualSize() const;

    void setActualSize(int actualSize);

    char* getDataAccess();

    std::string toString();

    char* operator[](int indice);
};


#endif //MIDDLEWARELOADER_BUFFER_H
