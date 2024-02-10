#pragma once
#include "InterfaceCommand.h"

class helpComm : public InterfaceCommand{

    int fd = 0;
    std::vector<std::string> commHelp;


public:
    helpComm(int, std::vector<std::string>); 

    void run() override;
    
    ~helpComm();
};