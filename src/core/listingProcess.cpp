// Oskar

#ifdef _WIN32
// Incluye las librerías necesarias para trabajar con procesos en Windows
#include <windows.h>     // API de Windows para manejar procesos
#include <tlhelp32.h>    // Para tomar snapshots del estado de los procesos activos
#include <iostream>      // Para imprimir mensajes en consola
#include <string>        // Para manejar nombres de procesos como cadenas
#include "ProcessManager.h" // Declaración de la clase ProcessManager
#include <iomanip>       // Para formatear la salida con setw y alineación

// Implementación del método listProcesses() de la clase ProcessManager
bool ProcessManager::listProcesses() {
    HANDLE hProcessSnap;         // Handle para el snapshot de procesos
    HANDLE hProcess;             // Handle para cada proceso individual
    PROCESSENTRY32 pe32;         // Estructura que contiene información de cada proceso
    DWORD dwPriorityClass;       // Prioridad del proceso

    // Toma un snapshot de todos los procesos activos en el sistema
    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE) {
        std::cerr << "Failed to take snapshot of processes. Error: " << GetLastError() << std::endl;
        return FALSE;
    }

    // Inicializa la estructura pe32 con el tamaño correcto
    pe32.dwSize = sizeof(PROCESSENTRY32);

    // Obtiene el primer proceso del snapshot
    if (!Process32First(hProcessSnap, &pe32)) {
        std::cerr << "Failed to get first process. Error: " << GetLastError() << std::endl;
        CloseHandle(hProcessSnap);
        return FALSE;
    }

    // Encabezado de la tabla de procesos
    std::cout << "------------------------------------------" << std::endl;
    std::cout << std::left << std::setw(10) << "PID"
              << std::setw(25) << "Process Name"
              << std::setw(15) << "Threads"
              << std::setw(15) << "Parent PID"
              << std::setw(15) << "Priority" << std::endl;
    std::cout << "------------------------------------------------------------------" << std::endl;

    // Itera sobre todos los procesos del snapshot
    do {
        dwPriorityClass = 0;

        // Abre el proceso para consultar su prioridad
        hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pe32.th32ProcessID);
        if (hProcess != NULL) {
            dwPriorityClass = GetPriorityClass(hProcess);
            if (!dwPriorityClass) {
                std::cerr << "GetPriorityClass failed. Error: " << GetLastError() << std::endl;
            }
            CloseHandle(hProcess); // Cierra el handle del proceso
        }

        // Convierte el nombre del ejecutable de wchar_t a std::string si es necesario
        #ifdef UNICODE
        std::wstring ws(pe32.szExeFile);
        std::string exeFile(ws.begin(), ws.end());
        #else
        std::string exeFile(pe32.szExeFile);
        #endif  

        // Imprime la información del proceso en formato de tabla
        std::cout << std::left
                  << std::setw(10) << pe32.th32ProcessID
                  << std::setw(25) << exeFile
                  << std::setw(15) << pe32.cntThreads
                  << std::setw(15) << pe32.th32ParentProcessID
                  << std::setw(15) << dwPriorityClass
                  << std::endl;

    } while (Process32Next(hProcessSnap, &pe32)); // Continúa con el siguiente proceso

    // Libera el snapshot
    CloseHandle(hProcessSnap);
    return TRUE; // Indica que la operación fue exitosa
}

//Pablo
#elif __linux__  
#include <sys/types.h>   // Define tipos de datos para el sistema (pid_t, etc.)
#include <dirent.h>      // Para manejar directorios (abrir, leer, cerrar /proc)
#include <string>        // Manejo de cadenas en C++
#include <iostream>      // Entrada y salida estándar (cout, cin)
#include <fstream>       // Para leer archivos (ifstream)
#include <vector>        // Uso de vectores dinámicos
#include <cctype>        // Funciones de clasificación de caracteres (isdigit)
#include <cstring>       // Funciones para manejo de cadenas estilo C
#include <unistd.h>      // Funciones POSIX (procesos, sistema)
#include <algorithm>     // Algoritmos estándar (all_of, etc.)
#include "ProcessManager.h" // Cabecera propia de la clase ProcessManager

// Implementación del método que lista los procesos en Linux
bool ProcessManager::listProcesses(){
    DIR *dir;                // Puntero al directorio
    struct dirent* ent;      // Estructura para leer entradas del directorio

    // Abrimos el directorio /proc, donde Linux guarda información de procesos
    dir = opendir("/proc");
    if(dir == NULL){         // Si no se pudo abrir
        perror("Could not open /proc"); // Muestra error
        return false;        // Se retorna false indicando fallo
    }

    // Encabezado de la tabla que mostrará procesos
    std::cout << "------------------------------------------" << std::endl;
    std::cout << "  PID\t\tProcess Name" << std::endl;
    std::cout << "------------------------------------------" << std::endl;

    // Leemos cada entrada dentro del directorio /proc
    while((ent = readdir(dir)) != NULL){
        std::string dname = ent->d_name; // Nombre de la entrada

        // Verificamos si el nombre es un número (PID de un proceso)
        bool is_pid = !dname.empty() && std::all_of(dname.begin(), dname.end(), ::isdigit);

        if(is_pid){ // Si la carpeta es un proceso
            std::string pid_str = dname;  // Guardamos el PID como string
            std::string status_path = "/proc/" + pid_str + "/status"; // Archivo con info del proceso
            std::ifstream status_file(status_path); // Abrimos el archivo status
            std::string line;

            if(status_file.is_open()){ // Si se pudo abrir el archivo
                while(std::getline(status_file, line)){ // Leemos línea por línea
                    // Buscamos la línea que empieza con "Name:"
                    if (line.substr(0, 5) == "Name:") {
                        std::string name = line.substr(6); // Extraemos el nombre del proceso
                        // Eliminamos espacios o tabulaciones al inicio
                        name.erase(0, name.find_first_not_of("\t "));

                        // Imprimimos el PID y el nombre del proceso
                        std::cout << " " << pid_str << "\t\t" << name << std::endl;
                        break; // Terminamos, no necesitamos leer más líneas
                    }
                }
                status_file.close(); // Cerramos el archivo
            }
        }
    }

    // Cerramos el directorio /proc
    closedir(dir);
    return true;
}

#endif
