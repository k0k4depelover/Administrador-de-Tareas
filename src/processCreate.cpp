#include "ProcessManager.h"

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

#endif

