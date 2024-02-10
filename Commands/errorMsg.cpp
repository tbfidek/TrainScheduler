#include "errorMsg.h"
#include "additionals.h"

errorMsg::errorMsg(int fileD) : fd(fileD) {}

void errorMsg::run()
{
    std::string errorM = "Command not found; please try again.";
    writeMsg(fd, errorM);
}

errorMsg::~errorMsg() {}