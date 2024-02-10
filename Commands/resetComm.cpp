#include "resetComm.h"
#include "Data.h"
#include "additionals.h"

resetComm::resetComm(int fileD, std::vector<std::string> comm, bool &log) : fd(fileD), commReset(comm), logged(log) {}

void resetComm::run()
{
    Data *d = d->getInstance();
    std::string resetMsg;
    // reset <train>
    // reset all

    if (logged == true)
    {
        if (commReset.size() != 2)
        {
            resetMsg = "Correct format is <reset <train>> or <reset all>. Please try again.";
        }
        else
        {
            if (commReset[1] == "all")
            {
                d->getData().clear();
                resetMsg = "The schedule has been reset.";
            }
            else
            {
                int foundT = returnTrain(commReset[1]);
                if (foundT > -1)
                {
                    d->getData().erase(commReset[1]);
                    resetMsg = "The schedule has been reset.";
                }
                else
                    resetMsg = "Correct format is <reset <train>> or <reset all>. Please try again.";
            }
        }
    }
    else resetMsg = "You need to be authenticated to use this command.\n";
    writeMsg(fd, resetMsg);
}

resetComm::~resetComm()
{
    commReset.clear();
}
