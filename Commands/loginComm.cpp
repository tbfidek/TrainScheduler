#include "loginComm.h"
#include "additionals.h"

std::string returnPassword(std::string trainId)
{
    std::string password;
    TiXmlDocument doc("baza.xml");
    TiXmlHandle handle(&doc);
    TiXmlElement *train;
    doc.LoadFile();
    int foundT = returnTrain(trainId);
    if (foundT > -1)
    {
        train = handle.Child(1).Child(foundT).Element();
        password = train->Attribute("Parola");
        return password;
    }
    return "";
}

loginComm::loginComm(int fileD, std::vector<std::string> login, bool &log) : fd(fileD), commLogin(login), loginSuccess(log)
{
    user = "";
    loginSuccess = log;
}

void loginComm::run()
{
    std::string loginMsg, password;

    if (loginSuccess == false)
    {

        if (commLogin.size() != 3)
        {
            loginMsg = "Command format is <login <user> <password>>. Please try again.\n";
        }
        else
        {
            TiXmlDocument doc("baza.xml");
            doc.LoadFile();
            password = returnPassword(commLogin[1]);

            if (password != "" && strcmp(commLogin[2].c_str(), password.c_str()) == 0)
            {
                loginSuccess = true;

                password = "";
                user = commLogin[1];
                loginMsg = "successfully logged in!\n";
            }
            else
                loginMsg = "wrong username or password.\n";
        }
    }
    else
        loginMsg = "you are already logged in\n";
    writeMsg(fd, loginMsg);
}

loginComm::~loginComm()
{
    commLogin.clear();
}