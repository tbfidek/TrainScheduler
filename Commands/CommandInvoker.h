#pragma once
#include "InterfaceCommand.h"
#include "showScheduleComm.h"
#include "errorMsg.h"
#include "loginComm.h"
#include "logoutComm.h"
#include "arrSpecTrainComm.h"
#include "depSpecTrainComm.h"
#include "delayComm.h"
#include "resetComm.h"
#include "arrivalsComm.h"
#include "departuresComm.h"
#include "helpComm.h"


class CommandInvoker
{
    char *comm;
    InterfaceCommand *ic;
    int fd = 0;
    bool &logged;
    pthread_mutex_t locked;


public:
    CommandInvoker(int, const char *, bool&, pthread_mutex_t&);
    void exec();
    std::vector<std::string> argcomm();
    void setcmd();
    ~CommandInvoker();
};