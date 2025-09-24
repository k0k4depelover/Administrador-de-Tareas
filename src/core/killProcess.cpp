//Leonel
#ifdef _WIN32
    // Inclusión de librerías específicas de Windows
    #include <windows.h>    // API de Windows: permite crear, manejar y terminar procesos
    #include <tlhelp32.h>   // Proporciona funciones para tomar "snapshots" (capturas) de procesos y threads
    #include <psapi.h>      // Permite obtener información sobre procesos y su uso de memoria
    #include <string>
    #include <iostream>
    #include "ProcessManager.h" // Cabecera propia con la definición de la clase ProcessManager

// Método de la clase ProcessManager para terminar un proceso dado su PID
bool ProcessManager::killProcess(int pid) {
    // Intenta abrir el proceso con el permiso de terminación
    HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pid);
   
    // Verifica si se pudo abrir el proceso
    if (hProcess == NULL) {
        std::cerr << "OpenProcess falló. (Error: " << GetLastError() << ")" << std::endl;
        return false; // Si no se abre, retorna false
    }

    // Intenta terminar el proceso con el código de salida 0
    if (!TerminateProcess(hProcess, 0)) {
        std::cout << "TerminateProcess falló. (Error: " << GetLastError() << ")" << std::endl;
        CloseHandle(hProcess); // Cierra el handle antes de salir
        return false;
    }

    // Si se terminó correctamente, se cierra el handle y retorna true
    CloseHandle(hProcess);
    return true;
}

//Alan
#elif __linux__
    //Por Implementar...
    #include <signal.h>
    #include <string>
    #include <iostream>
    #include <vector>
    #include "ProcessManager.h"
    
    bool ProcessManager::killProcess(int pid) {
        if(kill(pid, SIGTERM==-1)){
            perror("Kill failed");
            return false;
        }
        kill(pid, SIGKILL);
        return true;
    }

#endif