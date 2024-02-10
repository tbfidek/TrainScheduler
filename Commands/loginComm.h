#include "InterfaceCommand.h"

class loginComm : public InterfaceCommand
{
    std::vector<std::string> commLogin;
    int fd = 0;
    bool &loginSuccess;
    std::string user;

public:
    loginComm(int, std::vector<std::string>, bool&);

    void run() override;

    ~loginComm();
};