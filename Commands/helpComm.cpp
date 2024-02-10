#include "helpComm.h"
#include "additionals.h"

helpComm::helpComm(int fileD, std::vector<std::string> comm) : fd(fileD), commHelp(comm) {}

void helpComm::run()
{
    std::string information;

    if (commHelp.size() > 1)
        information = "Command format is <help>.";
    else
    {
        information += "To access the information about our trains, please use the following commands: \n";
        information = information + "\t - schedule : information about all trains\n"
                      "\t - schedule <train> : information about specific train \n"  
                      + "\t - arrivals   : trains arriving in the next hour\n" 
                      + "\t - departures   : trains leaving in the next hour\n" 
                      + "\t - departure <train> <station>\n" + "\t - arrival <train> <station>\n" + "Our operators are registering any possible delays during the day.\n" + "The schedule is automatically updated: delays are mentioned and the time is modified accordingly.\n";
    }
    writeMsg(fd, information);
}

helpComm::~helpComm()
{
    commHelp.clear();
}