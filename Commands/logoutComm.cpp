#include "logoutComm.h"
#include "additionals.h"

logoutComm::logoutComm(int fileD, std::vector<std::string> logout, bool &log) : fd(fileD), commLogout(logout), logoutStatus(log)
{
    logoutStatus = log;
}

void logoutComm::run()
{
    std::string logoutMsg;
    if (commLogout[0] == "logout")
    {
        if (logoutStatus == true)
        {
            logoutStatus = false;
            logoutMsg = "You've logged out!";
        }
        else
            logoutMsg = "You already logged out.";
    }
    writeMsg(fd, logoutMsg);
}

logoutComm::~logoutComm()
{
    commLogout.clear();
}
