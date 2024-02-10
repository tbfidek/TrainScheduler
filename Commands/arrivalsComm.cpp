#include "arrivalsComm.h"
#include "Data.h"
#include <string>
#include "additionals.h"

arrivalsComm::arrivalsComm(int fileD, std::vector<std::string> comm) : fd(fileD), commArr(comm) {}

void arrivalsComm::run()
{
    Data *d = d->getInstance();
    TiXmlDocument doc("baza.xml");
    TiXmlHandle handle(&doc);
    TiXmlHandle hRoot(0);
    TiXmlElement *train, *Stop, *Stations, *elemTrasa;

    doc.LoadFile();
    std::string trainId, arrTime, station, s, delay;
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
                if (Stop->LastChild()->ToElement()->Attribute("DenStaDestinatie") != elemTrasa->Attribute("DenStaDestinatie") && atoi(elemTrasa->Attribute("StationareSecunde")) > 0)
                {
                        if(elemTrasa->PreviousSibling()){
                            arrTime = elemTrasa->PreviousSibling()->ToElement()->Attribute("OraS");
                        }
                        s = elemTrasa->Attribute("DenStaOrigine");

                        if (d->getData().find(trainId) != d->getData().end())
                        {
                            if (d->getData().at(trainId).first == 1)
                            {

                                arrTime = std::to_string(atoi(arrTime.c_str()) + d->getData().at(trainId).second * 60);
                                delay = "\33[31m[ +";
                                delay += std::to_string(d->getData().at(trainId).second);
                                delay += " min ]\33[0m";
                            }
                            if (d->getData().at(trainId).first == 0)
                            {
                                arrTime = std::to_string(atoi(arrTime.c_str()) - d->getData().at(trainId).second * 60);
                                delay = "\33[31m[ -";
                                delay += std::to_string(d->getData().at(trainId).second);
                                delay += " min ]\33[0m";
                            }
                        }

                        time_t arr = atoi(arrTime.c_str());
                        struct tm ts;
                        struct tm current = getlocaltime();
                        ts = *localtime(&arr);
                        ts.tm_hour = arr / 3600 + 2;
                        mktime(&current);
                        mktime(&ts);

                        strftime(time, sizeof(time), "%H:%M", &ts);
                        arrTime = "";
                        arrTime.append(time);

                        if (compareTime(ts, current))
                        {
                            if (s.size() < 8)
                                s += "\t";
                            station = station + trainId + "\t--->\t" + s + "\t" + arrTime + "\t" + delay + "\n";
                        }
                }
                elemTrasa = elemTrasa->NextSiblingElement();
            }
        }
    }
    if (station == "")
    {
        station = "No trains are scheduled to arrive in the next hour.";
    }
    if (commArr.size() > 1)
    {
        station = "The correct command format is <arrivals>.\nTry <arrival <train> <station>> to retrieve more details.";
    }
    writeMsg(fd, station);
}

arrivalsComm::~arrivalsComm()
{
    commArr.clear();
}
