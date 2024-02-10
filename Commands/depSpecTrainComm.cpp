#include "depSpecTrainComm.h"
#include "Data.h"
#include <string>
#include "additionals.h"


depSpecTrainComm::depSpecTrainComm(int fileD, std::vector<std::string> trainDet) : fd(fileD), commDepDet(trainDet) {}

void depSpecTrainComm::run()
{
    Data *d = d->getInstance();
    TiXmlDocument doc("baza.xml");
    TiXmlHandle handle(&doc);
    TiXmlHandle hRoot(0);
    TiXmlElement *train, *Stop, *Stations, *elemTrasa;
    doc.LoadFile();
    std::string delay, depTime, location, departure;
    char time[80];
    int found = returnTrain(commDepDet[1]);
    int foundSt = 0;

    if (commDepDet.size() < 3 || commDepDet.size() > 5)
    {
        depTime = "Command format is: departure <id_train> <station>.\nTry <departures> for a general schedule.";
        writeMsg(fd, depTime);
    }
    else
    {
        // comanda: departure numar denstadestinatie => afisare oraS
        if (found != -1)
        {
            train = handle.Child(1).Child(found).Element();
            hRoot = TiXmlHandle(train);
            Stations = hRoot.Child(0).Element();
            Stop = hRoot.Child(0).FirstChild().Element();

            elemTrasa = Stop->FirstChildElement();
            location = commDepDet[2];
            if (commDepDet.size() == 5)
            {
                location = commDepDet[2] + " " + commDepDet[3] + " " + commDepDet[4];
            }
            if (commDepDet.size() == 4)
            {
                location = commDepDet[2] + " " + commDepDet[3];
            }
            if (Stations)
            {
                while (elemTrasa)
                {
                    if (strcmp(elemTrasa->Attribute("DenStaOrigine"), location.c_str()) == 0)
                    {
                        foundSt = 1;
                        depTime = elemTrasa->Attribute("OraP");

                        if (d->getData().find(commDepDet[1]) != d->getData().end())
                        {
                            if (d->getData().at(commDepDet[1]).first == 1)
                            {

                                depTime = std::to_string(atoi(depTime.c_str()) + d->getData().at(commDepDet[1]).second * 60);
                                delay = "\33[31m[ +";
                                delay += std::to_string(d->getData().at(commDepDet[1]).second);
                                delay += " min ]\33[0m";
                            }
                            if (d->getData().at(commDepDet[1]).first == 0)
                            {
                                depTime = std::to_string(atoi(depTime.c_str()) - d->getData().at(commDepDet[1]).second * 60);
                                delay = "\33[31m[ -";
                                delay += std::to_string(d->getData().at(commDepDet[1]).second);
                                delay += " min ]\33[0m";
                            }
                        }

                        time_t epochDep = atoi(depTime.c_str());
                        struct tm ts;
                        ts = *localtime(&epochDep);
                        ts.tm_hour = epochDep/3600 + 2;
                        strftime(time, sizeof(time), "%H:%M", &ts);
                        depTime = ""; 
                        depTime.append(time);

                        struct tm current = getlocaltime();
                        

                        if (delayThing(ts, current))
                        {
                            departure = commDepDet[1] + "\t" + depTime + "\t" + delay + "\n";
                        }
                        else
                        {
                            departure = commDepDet[1] + "\t" + depTime + "\n";
                        }
                        
                        break;
                    }
                    elemTrasa = elemTrasa->NextSiblingElement();
                }
                if (foundSt == 0)
                    departure = "The requested station was not found.";
            }
        }
        else
            departure = "The requested train was not found.";


        writeMsg(fd, departure);
    }
}

depSpecTrainComm::~depSpecTrainComm()
{
    commDepDet.clear();
}
