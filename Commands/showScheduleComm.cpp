#include "showScheduleComm.h"
#include "Data.h"
#include "additionals.h"

showScheduleComm::showScheduleComm(int fileD, std::vector<std::string> commSched) : fd(fileD), commSchedule(commSched) {}

void showScheduleComm::run()
{
    {
        Data *d = d->getInstance();
        TiXmlDocument doc("baza.xml");
        TiXmlHandle handle(&doc);
        TiXmlHandle hRoot(0);
        TiXmlElement *train, *Stop, *Stations, *elemTrasa;
        doc.LoadFile();
        std::string schedule, value, delay, arrTime;
        if (commSchedule.size() > 2)
        {
            schedule = "Command format is: <schedule train>. Please try again!\n";
        }
        else if (commSchedule.size() == 2)
        {
            int foundT = returnTrain(commSchedule[1]);
            if (foundT > -1)
            {
                train = handle.Child(1).Child(foundT).Element();
                hRoot = TiXmlHandle(train);

                Stations = hRoot.Child(0).Element(); // trase

                Stop = hRoot.Child(0).FirstChild().Element(); // trasa

                elemTrasa = Stop->FirstChildElement();

                if (Stations)
                {
                    schedule += "Train: " + commSchedule[1] + "\n";
                    while (elemTrasa)
                    {
                        if (atoi(elemTrasa->Attribute("StationareSecunde")) > 0)
                        {
                            std::string st = elemTrasa->Attribute("DenStaOrigine");
                            if (elemTrasa->PreviousSibling())
                            {
                                arrTime = elemTrasa->PreviousSibling()->ToElement()->Attribute("OraS");
                            }
                            else
                            {
                                arrTime = "0";
                            }
                            int stationminutes = atoi(elemTrasa->Attribute("StationareSecunde")) / 60;
                            std::string dpTime = elemTrasa->Attribute("OraP");

                            if (d->getData().find(commSchedule[1]) != d->getData().end())
                            {
                                if (d->getData().at(commSchedule[1]).first == 1)
                                {

                                    dpTime = std::to_string(atoi(dpTime.c_str()) + d->getData().at(commSchedule[1]).second * 60);
                                    arrTime = std::to_string(atoi(arrTime.c_str()) + d->getData().at(commSchedule[1]).second * 60);
                                    delay = "\33[31m[ +";
                                    delay += std::to_string(d->getData().at(commSchedule[1]).second);
                                    delay += " min ]\33[0m";
                                }
                                if (d->getData().at(commSchedule[1]).first == 0)
                                {
                                    dpTime = std::to_string(atoi(dpTime.c_str()) - d->getData().at(commSchedule[1]).second * 60);
                                    arrTime = std::to_string(atoi(arrTime.c_str()) - d->getData().at(commSchedule[1]).second * 60);
                                    delay = "\33[31m[ -";
                                    delay += std::to_string(d->getData().at(commSchedule[1]).second);
                                    delay += " min ]\33[0m";
                                }
                            }
                            char timp[6], timpdp[6];

                            time_t dp = atoi(dpTime.c_str());
                            time_t arrival = atoi(arrTime.c_str());

                            struct tm depTime;

                            depTime = *localtime(&dp);
                            depTime.tm_hour = dp / 3600 + 2;
                            strftime(timpdp, sizeof(timpdp), "%H:%M", &depTime);
                            dpTime = "";
                            dpTime.append(timpdp);

                            struct tm arTime;
                            arTime = *localtime(&arrival);
                            arTime.tm_hour = arrival / 3600 + 2;
                            strftime(timp, sizeof(timp), "%H:%M", &arTime);
                            arrTime = "";
                            arrTime.append(timp);

                            struct tm current = getlocaltime();
                            mktime(&current);
                            mktime(&depTime);
                            mktime(&arTime);

                            if (st.size() < 13)
                                st += "\t";

                            if (delayThing(depTime, current))
                            {
                                if (Stop->LastChild()->ToElement()->Attribute("DenStaOrigine") == elemTrasa->Attribute("DenStaOrigine"))
                                {
                                    schedule += "Station:   " + st + "\tArrives at " + dpTime + "\t\t\t\t\t\t" + delay + "\n";
                                }
                                else if (Stop->FirstChildElement()->Attribute("DenStaOrigine") == elemTrasa->Attribute("DenStaOrigine"))
                                {
                                    schedule += "Station:   " + st + "\tLeaves  at " + "\t" + dpTime + "\t\t\t\t\t\t" + delay + "\n";
                                }
                                else
                                {
                                    schedule += "Station:   " + st + "\tArrives at " + arrTime + "  |  " + "Stays " + std::to_string(stationminutes) + " minute(s)" + "  |  " + "Leaves at " + dpTime + "\t" + delay + "\n";
                                }
                            }
                            else
                            {
                                if (Stop->LastChild()->ToElement()->Attribute("DenStaOrigine") == elemTrasa->Attribute("DenStaOrigine"))
                                {
                                    schedule += "Station:   " + st + "\tArrives at " + dpTime + "\n";
                                }
                                else if (Stop->FirstChildElement()->Attribute("DenStaOrigine") == elemTrasa->Attribute("DenStaOrigine"))
                                {
                                    schedule += "Station:   " + st + "\tLeaves  at " + dpTime + "\n";
                                }
                                else
                                {
                                    schedule += "Station:   " + st + "\tArrives at " + arrTime + "  |  " + "Stays " + std::to_string(stationminutes) + " minute(s)" + "  |  " + "Leaves at " + dpTime + "\n";
                                }
                            }
                        }
                        elemTrasa = elemTrasa->NextSiblingElement();
                    }
                }
            }
            else
            {
                schedule = "This train does not exist. Please try another train id.\n";
            }
        }
        else
        {
            schedule = showSchedule();
        }
        writeMsg(fd, schedule);
    }
}

showScheduleComm::~showScheduleComm()
{
    commSchedule.clear();
}