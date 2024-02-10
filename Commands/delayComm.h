#pragma once
#include "InterfaceCommand.h"

class delayComm : public InterfaceCommand
{
    int fd;
    std::vector<std::string> commDelay;
    bool& logged;

public:

    delayComm(int, std::vector<std::string>, bool&);
    void run();
    ~delayComm();
};