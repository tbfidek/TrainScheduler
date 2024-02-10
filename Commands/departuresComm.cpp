#include "departuresComm.h"
#include "Data.h"
#include <string>
#include "additionals.h"

departuresComm::departuresComm(int fileD, std::vector<std::string> comm) : fd(fileD), commDep(comm) {}

void departuresComm::run()
{
    Data *d = d->getInstance();
    TiXmlDocument doc("baza.xml");
    TiXmlHandle handle(&doc);
    TiXmlHandle hRoot(0);
    TiXmlElement *train, *Stop, *Stations, *elemTrasa;

    doc.LoadFile();
    std::string trainId, depTime, station, s, delay;
    char time[10];
    for (int i = 0; i < 60; i++)
    {
        delay = "\33[32m[ no delay ]\33[0m";
        train = handle.Child(1).Child(i).Element();
        if (!train)
            printf("error");
        hRoot = TiXmlHandle(train);
        trainId = train->Attribute("Numar");
        Stations = hRoot.Child(0).Element();
        Stop = hRoot.Child(0).FirstChild().Element();
        elemTrasa = Stop->FirstChildElement();
        if (Stations)
        {
            while (elemTrasa)
            {
                if (Stop->LastChild()->ToElement()->Attribute("DenStaOrigine") != elemTrasa->Attribute("DenStaOrigine") && atoi(elemTrasa->Attribute("StationareSecunde")) > 0)
                {
                    depTime = elemTrasa->Attribute("OraP");
                    s = elemTrasa->Attribute("DenStaOrigine");

                    if (d->getData().find(trainId) != d->getData().end())
                    {
                        if (d->getData().at(trainId).first == 1)
                        {

                            depTime = std::to_string(atoi(depTime.c_str()) + d->getData().at(trainId).second * 60);
                            delay = "\33[31m[ +";
                            delay += std::to_string(d->getData().at(trainId).second);
                            delay += " min ]\33[0m";
                        }
                        if (d->getData().at(trainId).first == 0)
                        {
                            depTime = std::to_string(atoi(depTime.c_str()) - d->getData().at(trainId).second * 60);
                            delay = "\33[31m[ -";
                            delay += std::to_string(d->getData().at(trainId).second);
                            delay += " min ]\33[0m";
                        }
                    }
                    time_t arr = atoi(depTime.c_str());
                    struct tm ts;
                    struct tm current = getlocaltime();
                    ts = *localtime(&arr);
                    ts.tm_hour = arr / 3600 + 2;
                    mktime(&current);
                    mktime(&ts);

                    strftime(time, sizeof(time), "%H:%M", &ts);
                    depTime = "";
                    depTime.append(time);

                    if (compareTime(ts, current))
                    {
                        if (s.size() < 8)
                            s += "\t";
                        station = station + trainId + "\t--->\t" + s + "\t" + depTime + "\t" + delay + "\n";
                    }
                }

                elemTrasa = elemTrasa->NextSiblingElement();
            }
        }
    }
    if (station == "")
    {
        station = "No trains are scheduled to leave in the next hour.";
    }
    if (commDep.size() > 1)
    {
        station = "The correct command format is <departures>.\nTry <departure <train> <station>> to retrieve more details.";
    }
    writeMsg(fd, station);
}

departuresComm::~departuresComm()
{
    commDep.clear();
}
