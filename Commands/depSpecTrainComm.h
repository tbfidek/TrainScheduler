#include "InterfaceCommand.h"

class depSpecTrainComm : public InterfaceCommand
{
    std::vector<std::string> commDepDet;
    int fd = 0;

public:
    depSpecTrainComm(int, std::vector<std::string>);

    void run() override;

    ~depSpecTrainComm();
};