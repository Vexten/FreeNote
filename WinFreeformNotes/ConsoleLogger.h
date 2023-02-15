#pragma once
#include "ILogger.h"
#include <iostream>

class ConsoleLogger :
    public ILogger
{
public:
    ConsoleLogger() {}

    bool write(std::string str) {
        std::cout << str << std::endl;
        return true;
    }
};

