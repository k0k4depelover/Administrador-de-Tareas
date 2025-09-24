#include "ProcessManager.h"

//Oskar
#ifdef _WIN32
    #include <windows.h> //Windows API para crear, manejar y terminar procesos
    #include <tlhelp32.h> //Para tomar snapshots de los procesos
    #include <psapi.h> //Para obtener información sobre procesos
    #include <string>
    #include <iostream>


int ProcessManager::runProcess(const std::string& cmd) {
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb=sizeof(si); 
    if(!CreateProcessA(NULL,
                        (LPSTR)cmd.c_str(),
                        NULL,
                        NULL,
                        FALSE,
                        0,
                        NULL,
                        NULL,
                        &si,
                        &pi )){
        std::cerr<<"Create process failed. ("<< GetLastError()<<")"<<std::endl;
        return -1;                    

    }
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    return pi.dwProcessId;
}

//Donald
/*
Este código implementa en Linux un manejador de procesos dentro de la clase ProcessManager.
Divide comandos en argumentos (splitCommand).

Crea procesos hijos usando fork().

En el hijo ejecuta el comando con execvp().

En el padre, si es un comando corto (date, echo, whoami), espera a que termine con waitpid().

Devuelve el PID del proceso creado. 
*/
#elif _linux_
    #include <string>
    #include <iostream>
    #include <vector>
    #include "ProcessManager.h"
    #include <unistd.h>    
    #include <sys/types.h>
    #include <sys/wait.h>
    #include <cstring>

    std::vector<std::string>  ProcessManager::splitCommand(const std::string& cmd) {
        std::vector<std::string> args;
        std::string current_arg;
        for(char c: cmd){
            if(c== ' ' && !current_arg.empty()){
                args.push_back(current_arg);
                current_arg.clear();
            }
            else{
                current_arg+=c;
            }
            
        }
        if(!current_arg.empty()){
            args.push_back(current_arg);
        }
        return args;
    }

    int ProcessManager::runProcess(const std::string& cmd){
        pid_t pid= fork();
        if(pid==-1){
            perror("Fork failed");
            return -1;
        }
        else if(pid==0){
            std::vector<std::string> args=splitCommand(cmd);
            std::vector<char*> c_args;
            for(const auto& arg : args){
                c_args.push_back(const_cast<char*>(arg.c_str()));
            }
            c_args.push_back(nullptr);
            execvp(c_args[0], c_args.data());
            perror("Execvp failed");
            _exit(1);

    }

        else{
            if(isShortCommand(cmd)){
                int status;
                waitpid(pid, nullptr, 0);
            }
            std::vector <pid_t> child_pids;
            child_pids.push_back(pid);
            return pid;
        }

    }
    bool ProcessManager::isShortCommand(const std::string& cmd){
        return cmd.starts_with("date") || cmd.starts_with("echo") || cmd.starts_with("whoami");}
#endif

