#include "InterfaceCommand.h"

class departuresComm : public InterfaceCommand
{
    std::vector<std::string> commDep;
    int fd = 0;

public:
    departuresComm(int, std::vector<std::string>);

    void run() override;

    ~departuresComm();
};