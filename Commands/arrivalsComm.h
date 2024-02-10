#include "InterfaceCommand.h"

class arrivalsComm : public InterfaceCommand
{
    std::vector<std::string> commArr;
    int fd = 0;

public:
    arrivalsComm(int, std::vector<std::string>);

    void run() override;

    ~arrivalsComm();
};