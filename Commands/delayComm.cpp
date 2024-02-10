#include "delayComm.h"
#include "Data.h"
#include "additionals.h"

delayComm::delayComm(int fileD, std::vector<std::string> delaycomm, bool &log) : fd(fileD), commDelay(delaycomm), logged(log) {}

void delayComm::run()
{
    Data *d = d->getInstance();
    TiXmlDocument doc("baza.xml");
    TiXmlHandle handle(&doc);
    TiXmlHandle hRoot(0);
    TiXmlElement *train;
    doc.LoadFile();
    std::string delayMsg;
    int delay = 0, sign = 0;
    std::pair<int, int> pair;
    if (logged)
    {
        if (commDelay.size() != 3)
        {
            delayMsg = "Command format is <delay <train> <+/-number>>. Please try again.";
        }
        else if (commDelay[2].find("+") == std::string::npos && commDelay[2].find("-") == std::string::npos)
        {
            delayMsg = "Command format is <delay <train> <+/-number>>. Please try again.";
        }
        else
        {
            int foundT = returnTrain(commDelay[1]);
            if (foundT > -1)
            {
                train = handle.Child(1).Child(foundT).Element();
                hRoot = TiXmlHandle(train);
                if (!train)
                    printf("error");

                // delay idtren +/-min
                //  0 => - (early)
                //  1 => + (late)

                if (commDelay[2].find("+") != std::string::npos)
                {
                    sign = 1;
                    commDelay[2].erase(0, 1);
                    delay = atoi(commDelay[2].c_str());
                    pair = std::make_pair(sign, delay);
                }
                else if (commDelay[2].find("-") != std::string::npos)
                {
                    sign = 0;
                    commDelay[2].erase(0, 1);
                    delay = atoi(commDelay[2].c_str());
                    pair = std::make_pair(sign, delay);
                }
                if(d->getData().count(commDelay[1]) > 0){
                    d->getData().erase(commDelay[1]);
                }
                d->getData().insert({commDelay[1], pair});

                delayMsg = "Train was updated with a " + commDelay[2] + " minute delay. Thank you!";
            }
            else
                delayMsg = "Train not found; cannot modify schedule.";
        }
    }
    else
    {
        delayMsg = "You need to be authenticated to use this command.\n";
    }
    writeMsg(fd, delayMsg);
}

delayComm::~delayComm()
{

    commDelay.clear();
}
