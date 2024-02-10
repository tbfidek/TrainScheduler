#pragma once
#include "time.h"
#include "Data.h"

inline struct tm getlocaltime()
{
    time_t now = time(NULL);
    struct tm time = *localtime(&now);
    time.tm_hour += 7;
    mktime(&time);

    return time;
}

inline int compareTime(struct tm t1, struct tm t2)
{
    // t1 = ts
    // t2 = current
    if (t1.tm_hour == t2.tm_hour && t1.tm_min >= t2.tm_min)
    {
        return 1;
    }
    else if (t1.tm_hour > t2.tm_hour && t1.tm_min <= t2.tm_min)
    {
        t2.tm_hour += 1;
        if (t1.tm_hour <= t2.tm_hour)
        {
            return 1;
        }
    }
    return 0;
}

inline int delayThing(struct tm t1, struct tm t2)
{
    // t1 = departure time
    // t2 = current time
    if (t1.tm_hour == t2.tm_hour && t1.tm_min >= t2.tm_min)
    {
        return 1;
    }
    if (t1.tm_hour > t2.tm_hour)
        return 1;
    // 1 = current time < departure
    return 0;
}

inline int returnTrain(std::string name)
{
    TiXmlDocument doc("baza.xml");
    TiXmlHandle handle(&doc);
    TiXmlHandle hRoot(0);
    TiXmlElement *train;
    doc.LoadFile();
    std::string trainId;
    int found = -1;
    for (int i = 0; i < 60; i++)
    {
        train = handle.Child(1).Child(i).Element();
        hRoot = TiXmlHandle(train);
        trainId = train->Attribute("Numar");
        if (trainId == name)
        {
            found = i;
        }
    }
    return found;
}
inline void writeMsg(int fileD, std::string name)
{
    int size = name.size();
    if (write(fileD, &size, sizeof(size)) <= 0)
    {
        perror("[Thread] Eroare la write() catre client.");
    }
    if (write(fileD, name.c_str(), strlen(name.c_str())) <= 0)
    {
        perror("[Thread] Eroare la write() catre client.");
    }
}

inline std::string showSchedule()
{
    Data *d = d->getInstance();
    TiXmlDocument doc("baza.xml");
    TiXmlHandle handle(&doc);
    TiXmlHandle hRoot(0);
    TiXmlElement *train, *arrStop, *depStop, *Stop;
    std::string officialSchedule;
    doc.LoadFile();
    std::string trainId, arrTime, arrSt, depTime, depSt, delay, delayD;

    char timeDep[10], timeArr[10];

    int found = -1;
    int foundSt = 0;
    officialSchedule += "------------------------------------------------------------------------------------------------------------\n";
    officialSchedule += "Train\tDeparture Station    Departure     Delay\t\tArrival Station\t\tArrival     Delay\n";
    officialSchedule += "------------------------------------------------------------------------------------------------------------\n";

    for (int i = 0; i < 60; i++)
    {
        delay = "\33[32m[ no delay ]\33[0m";
        delayD = "\33[32m[ no delay ]\33[0m";

        train = handle.Child(1).Child(i).Element();
        if (!train)
            arrTime = "error";
        hRoot = TiXmlHandle(train);
        Stop = hRoot.Child(0).FirstChild().Element();
        depStop = Stop->FirstChildElement();
        arrStop = Stop->LastChild()->ToElement();

        trainId = train->Attribute("Numar");

        arrSt = arrStop->Attribute("DenStaOrigine");
        depSt = depStop->Attribute("DenStaOrigine");

        arrTime = arrStop->Attribute("OraS");
        depTime = depStop->Attribute("OraP");

        if (d->getData().find(trainId) != d->getData().end())
        {
            time_t rn = time(NULL);
            struct tm rightnow = *localtime(&rn);
            rightnow.tm_hour += 7;

            time_t dep = atoi(depTime.c_str());
            struct tm depT = *localtime(&dep);
            depT.tm_hour = dep / 3600 + 2;
            mktime(&depT);
            mktime(&rightnow);

            if (d->getData().at(trainId).first == 1)
            {
                arrTime = std::to_string(atoi(arrTime.c_str()) + d->getData().at(trainId).second * 60);

                if (depT.tm_hour >= rightnow.tm_hour)
                {
                    depTime = std::to_string(atoi(depTime.c_str()) + d->getData().at(trainId).second * 60);
                    delayD = "\33[31m[ +";
                    delayD += std::to_string(d->getData().at(trainId).second);
                    delayD += " min ]\33[0m";
                }
                delay = "\33[31m[ +";
                delay += std::to_string(d->getData().at(trainId).second);
                delay += " min ]\33[0m";
            }
            if (d->getData().at(trainId).first == 0)
            {
                if (depT.tm_hour >= rightnow.tm_hour)
                {
                    depTime = std::to_string(atoi(depTime.c_str()) - d->getData().at(trainId).second * 60);
                    delayD = "\33[31m[ -";
                    delayD += std::to_string(d->getData().at(trainId).second);
                    delayD += " min ]\33[0m";
                }
                arrTime = std::to_string(atoi(arrTime.c_str()) - d->getData().at(trainId).second * 60);
                delay = "\33[31m[ -";
                delay += std::to_string(d->getData().at(trainId).second);
                delay += " min ]\33[0m";
            }
        }

        time_t arrival, departure;
        struct tm t;

        arrival = atoi(arrTime.c_str());
        departure = atoi(depTime.c_str());

        t = *localtime(&departure);
        t.tm_hour = departure / 3600 + 2;
        mktime(&t);

        strftime(timeDep, sizeof(timeDep), "%H:%M", &t);
        depTime = "";
        depTime.append(timeDep);

        t = *localtime(&arrival);
        t.tm_hour = arrival / 3600 + 2;
        mktime(&t);

        strftime(timeArr, sizeof(timeArr), "%H:%M", &t);
        arrTime = "";
        arrTime.append(timeArr);

        if (depSt.size() < 8)
            depSt += "\t\t\t";
        else if (depSt.size() >= 8 && depSt.size() < 16)
            depSt += "\t\t";
        else
            depSt += "\t";
        if (arrSt.size() < 8)
            arrSt += "\t\t\t";
        else if (arrSt.size() >= 8 && arrSt.size() < 16)
            arrSt += "\t\t";
        else
            arrSt += "\t";

        officialSchedule += trainId + "\t" + depSt + depTime + "\t" + delayD + "\t-->\t" + arrSt + arrTime + "\t" + delay + "\n";
    }
    officialSchedule += "------------------------------------------------------------------------------------------------------------\n";
    officialSchedule += "\t\t\t\t\t T R A I N    S C H E D U L E\t\t\t\t\n";
    officialSchedule += "------------------------------------------------------------------------------------------------------------\n";

    return officialSchedule;
}
