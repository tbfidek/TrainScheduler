#include "InterfaceCommand.h"

class resetComm : public InterfaceCommand
{
    std::vector<std::string> commReset;
    int fd = 0;
    bool &logged;

public:
    resetComm(int, std::vector<std::string>, bool&);

    void run() override;

    ~resetComm();
};