//Oskar
#ifndef PROCESSMANAGER_H
#define PROCESSMANAGER_H
#include <string>
#include <vector>
class ProcessManager{
    public:
    int runProcess(const std::string& cmd);
    bool killProcess(int pid);
    std::vector<std::string> splitCommand(const std::string& cmd);
    bool listProcesses();
    bool isShortCommand(const std::string& cmd);
};
#endif // PROCESSMANAGER_HW
