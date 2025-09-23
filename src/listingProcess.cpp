#ifdef _WIN32
#include <windows.h> //Windows API para crear, manejar y terminar procesos
#include <tlhelp32.h> //Para tomar snapshots de los procesos
#include <iostream>
#include <string>
#include "ProcessManager.h"
#include <iomanip>
bool ProcessManager::listProcesses(){
    HANDLE hProcessSnap;
    HANDLE hProcess;
    PROCESSENTRY32 pe32;
    DWORD dwPriorityClass;
    hProcessSnap=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if(hProcessSnap== INVALID_HANDLE_VALUE){
        std::cerr<<"Failed to take snapshot of processes."<<GetLastError()<<std::endl;
        return (FALSE);

    }
    pe32.dwSize= sizeof(PROCESSENTRY32);

    if(!Process32First(hProcessSnap, &pe32)){
        std::cerr<<"Failed to get first process."<<GetLastError()<<std::endl;
        CloseHandle(hProcessSnap);
        return(FALSE);
    }
    std::cout << "------------------------------------------" << std::endl;
    std::cout << std::left << std::setw(10) << "PID"
            << std::setw(25) << "Process Name"
            << std::setw(15) << "Threads"
            << std::setw(15) << "Parent PID"
            << std::setw(15) << "Priority" << std::endl;
    std::cout << "------------------------------------------------------------------" << std::endl;

do {
    dwPriorityClass = 0;
    hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pe32.th32ProcessID);
    if (hProcess != NULL) {
        dwPriorityClass = GetPriorityClass(hProcess);
        if (!dwPriorityClass) {
            std::cerr << "GetPriorityClass failed. Error: " << GetLastError() << std::endl;
        }
        CloseHandle(hProcess);
    }

    // Convertir wchar_t* a std::string
    #ifdef UNICODE
    std::wstring ws(pe32.szExeFile);
    std::string exeFile(ws.begin(), ws.end());
    #else
    std::string exeFile(pe32.szExeFile);
    #endif  
    // Imprimir todo alineado
    std::cout << std::left
              << std::setw(10) << pe32.th32ProcessID
              << std::setw(25) << exeFile
              << std::setw(15) << pe32.cntThreads
              << std::setw(15) << pe32.th32ParentProcessID
              << std::setw(15) << dwPriorityClass
              << std::endl;

} while (Process32Next(hProcessSnap, &pe32));


    CloseHandle(hProcessSnap);
    return(TRUE);
}


#endif