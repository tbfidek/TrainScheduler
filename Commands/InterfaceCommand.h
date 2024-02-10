#pragma once
#include "./files/tinyxml.h"
#include <unistd.h>
#include <vector>

class InterfaceCommand
{
public:
    virtual ~InterfaceCommand() {}
    virtual void run() = 0;
};