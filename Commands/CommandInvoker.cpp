#include "CommandInvoker.h"
#include "additionals.h"

CommandInvoker::CommandInvoker(int fd, const char *command, bool &logd,  pthread_mutex_t &lock) : logged(logd)
{
    this->fd = fd;
    comm = (char *)malloc(strlen(command) * sizeof(char));
    strcpy(comm, command);
    locked = lock;
}

void CommandInvoker::exec()
{
    if (ic)
    {
        ic->run();
    }
}

std::vector<std::string> CommandInvoker::argcomm()
{

    // spargem comanda sheesh
    std::vector<std::string> commDetails;
    char *token;
    token = strtok(comm, " ");
    commDetails.push_back(token);
    token = strtok(NULL, " ");
    while (token != NULL)
    {
        commDetails.push_back(token);
        token = strtok(NULL, " ");
    }

    return commDetails;
}

void CommandInvoker::setcmd()
{
    std::vector<std::string> commDet = argcomm();
    if (strcmp("schedule", commDet[0].c_str()) == 0)
    {
        ic = new showScheduleComm(fd, commDet);
    }
    else if (strcmp("quit", commDet[0].c_str()) == 0)
    {
        logged = false;
        writeMsg(fd, "quit");
    }
    else if (strcmp("login", commDet[0].c_str()) == 0)
    {
        printf("%d\n", logged);
        ic = new loginComm(fd, commDet, logged);
    }
    else if (strcmp("logout", commDet[0].c_str()) == 0)
    {
        ic = new logoutComm(fd, commDet, logged);
    }
    else if (strcmp("arrival", commDet[0].c_str()) == 0)
    {

        ic = new arrSpecTrainComm(fd, commDet);
    }
    else if (strcmp("departure", commDet[0].c_str()) == 0)
    {

        ic = new depSpecTrainComm(fd, commDet);
    }
    else if (strcmp("arrivals", commDet[0].c_str()) == 0)
    {

        ic = new arrivalsComm(fd, commDet);
    }
    else if (strcmp("departures", commDet[0].c_str()) == 0)
    {

        ic = new departuresComm(fd, commDet);
    }
    else if (strcmp("delay", commDet[0].c_str()) == 0)
    {
        pthread_mutex_lock(&locked);
        ic = new delayComm(fd, commDet, logged);
        pthread_mutex_unlock(&locked);

    }
    else if (strcmp("reset", commDet[0].c_str()) == 0)
    {
        pthread_mutex_lock(&locked);
        ic = new resetComm(fd, commDet, logged);
        pthread_mutex_unlock(&locked);

    }
    else if (strcmp("help", commDet[0].c_str()) == 0)
    {
        ic = new helpComm(fd, commDet);
    }
    else
    {
        ic = new errorMsg(fd);
    }
};

CommandInvoker::~CommandInvoker()
{
    ic = nullptr;
};
