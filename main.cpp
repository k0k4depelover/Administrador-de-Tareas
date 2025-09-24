#include <iostream>
#include "core/ProcessManager.h"
#include <string>

#ifdef _WIN32
    #include <minwindef.h>
    typedef DWORD pid_type;
#else
    #include <unistd.h>
    typedef pid_t pid_type;
#endif

int main(){
    ProcessManager pm;
    std::string command;
    std::string input_arg;

    while(true){
        std::cout << "------------------------------------------" << std::endl;
        std::cout << "Enter command (run, kill, list, exit): ";
        std::cin >> command;

        if(command == "run"){
            std::cin.ignore();
            std::cout << "Enter command to run: ";
            std::getline(std::cin, input_arg);
            pid_type pid = pm.runProcess(input_arg);
            if(pid != -1){
                std::cout << "Process started with PID: " << pid << std::endl;
                std::cout<< std::endl;
            }else{
                std::cout << "Failed to start process." << std::endl;
            }
        }else if(command == "kill"){
            pid_type pid;
            std::cout << "Enter PID to kill: ";
            std::cin >> pid;
            if(pm.killProcess(pid)){
                std::cout << "Process " << pid << " killed successfully." << std::endl;
            }else{
                std::cout << "Failed to kill process " << pid << "." << std::endl;
            }
        }else if(command == "list"){
            pm.listProcesses();
        }else if(command == "exit"){
            break;
        }else{
            std::cout << "Unknown command. Please try again." << std::endl;
        }
    }
}