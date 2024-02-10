#include "arrSpecTrainComm.h"
#include "Data.h"
#include <string>
#include "additionals.h"

arrSpecTrainComm::arrSpecTrainComm(int fileD, std::vector<std::string> trainDet) : fd(fileD), commArrDet(trainDet) {}

void arrSpecTrainComm::run()
{
    Data *d = d->getInstance();
    TiXmlDocument doc("baza.xml");
    TiXmlHandle handle(&doc);
    TiXmlHandle hRoot(0);
    TiXmlElement *train, *Stop, *Stations, *elemTrasa;
    doc.LoadFile();
    std::string arrTime, location, delay, arrival;
    char time[80];
    int found = returnTrain(commArrDet[1]);
    int foundSt = 0;

    if (commArrDet.size() < 3 || commArrDet.size() > 5)
    {
        arrTime = "Command format is: arrival <id_train> <station>\nTry <arrivals> for a general schedule.";
        writeMsg(fd, arrival);
    }
    else
    {
        // comanda: arrival numar denstadestinatie => afisare oraS
        if (found != -1)
        {
            train = handle.Child(1).Child(found).Element();
            hRoot = TiXmlHandle(train);
            Stations = hRoot.Child(0).Element();
            Stop = hRoot.Child(0).FirstChild().Element();
            elemTrasa = Stop->FirstChildElement();
            location = commArrDet[2];
            if (commArrDet.size() == 5)
            {
                location = commArrDet[2] + " " + commArrDet[3] + " " + commArrDet[4];
            }
            if (commArrDet.size() == 4)
            {
                location = commArrDet[2] + " " + commArrDet[3];
            }
            if (Stations)
            {

                while (elemTrasa)
                {
                    if (strcmp(elemTrasa->Attribute("DenStaDestinatie"), location.c_str()) == 0)
                    {
                        foundSt = 1;
                        arrTime = elemTrasa->Attribute("OraS");

                        if (d->getData().find(commArrDet[1]) != d->getData().end())
                        {
                            if (d->getData().at(commArrDet[1]).first == 1)
                            {

                                arrTime = std::to_string(atoi(arrTime.c_str()) + d->getData().at(commArrDet[1]).second * 60);
                                delay = "\33[31m[ +";
                                delay += std::to_string(d->getData().at(commArrDet[1]).second);
                                delay += " min ]\33[0m";
                            }
                            if (d->getData().at(commArrDet[1]).first == 0)
                            {
                                arrTime = std::to_string(atoi(arrTime.c_str()) - d->getData().at(commArrDet[1]).second * 60);
                                delay = "\33[31m[ -";
                                delay += std::to_string(d->getData().at(commArrDet[1]).second);
                                delay += " min ]\33[0m";
                            }
                        }
                        time_t epochArr = atoi(arrTime.c_str());
                        struct tm ts;
                        ts = *localtime(&epochArr);
                        ts.tm_hour = epochArr / 3600 + 2;
                        strftime(time, sizeof(time), "%H:%M", &ts);
                        arrTime = "";
                        arrTime.append(time);

                        struct tm current = getlocaltime();
                        mktime(&ts);

                        if (delayThing(ts, current))
                        {
                            arrival = commArrDet[1] + "\t" + arrTime + "\t" + delay + "\n";
                        }
                        else
                        {
                            arrival = commArrDet[1] + "\t" + arrTime + "\n";
                        }

                        break;
                    }
                    elemTrasa = elemTrasa->NextSiblingElement();
                }
                if (foundSt == 0)
                    arrival = "The requested station was not found.";
            }
        }
        else
            arrival = "The requested train was not found.";

        writeMsg(fd, arrival);
    }
}

arrSpecTrainComm::~arrSpecTrainComm()
{
    commArrDet.clear();
}
