#pragma once
#include "InterfaceCommand.h"

class showScheduleComm : public InterfaceCommand
{
    std::vector<std::string> commSchedule;
    int fd = 0;

public:
    showScheduleComm(int, std::vector<std::string>); 

    void run() override;
    
    ~showScheduleComm();
};
