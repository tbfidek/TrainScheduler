#include "InterfaceCommand.h"

class arrSpecTrainComm : public InterfaceCommand
{
    std::vector<std::string> commArrDet;
    int fd = 0;

public:
    arrSpecTrainComm(int, std::vector<std::string>);

    void run() override;

    ~arrSpecTrainComm();
};