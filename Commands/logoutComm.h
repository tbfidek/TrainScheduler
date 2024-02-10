#include "InterfaceCommand.h"

class logoutComm : public InterfaceCommand
{
    std::vector<std::string> commLogout;
    int fd = 0;
    bool &logoutStatus;

public:
    logoutComm(int, std::vector<std::string>, bool&);

    void run() override;

    ~logoutComm();
};