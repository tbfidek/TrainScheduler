#pragma once
#include "InterfaceCommand.h"

class errorMsg : public InterfaceCommand{

    int fd = 0;

public:
    errorMsg(int); 

    void run() override;
    
    ~errorMsg();
};
