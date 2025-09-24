#include <iostream>              // Para entrada/salida por consola
#include "core/ProcessManager.h" // Incluye la clase que gestiona procesos
#include <string>                // Para manejar comandos como cadenas

// Definición multiplataforma del tipo de PID
#ifdef _WIN32
    #include <minwindef.h>       // Define DWORD en Windows
    typedef DWORD pid_type;      // En Windows, el PID es un DWORD
#else
    #include <unistd.h>          // En sistemas Unix, incluye pid_t
    typedef pid_t pid_type;      // En Unix, el PID es un pid_t
#endif

int main() {
    ProcessManager pm;           // Instancia del gestor de procesos
    std::string command;         // Comando ingresado por el usuario
    std::string input_arg;       // Argumento adicional para el comando

    // Bucle principal del programa: espera comandos del usuario
    while (true) {
        std::cout << "------------------------------------------" << std::endl;
        std::cout << "Enter command (run, kill, list, exit): ";
        std::cin >> command;

        // Ejecutar un nuevo proceso
        if (command == "run") {
            std::cin.ignore();   // Limpia el buffer para leer línea completa
            std::cout << "Enter command to run: ";
            std::getline(std::cin, input_arg); // Lee el comando completo
            pid_type pid = pm.runProcess(input_arg); // Ejecuta el proceso

            if (pid != -1) {
                std::cout << "Process started with PID: " << pid << std::endl << std::endl;
            } else {
                std::cout << "Failed to start process." << std::endl;
            }

        // Terminar un proceso por su PID
        } else if (command == "kill") {
            pid_type pid;
            std::cout << "Enter PID to kill: ";
            std::cin >> pid;

            if (pm.killProcess(pid)) {
                std::cout << "Process " << pid << " killed successfully." << std::endl;
            } else {
                std::cout << "Failed to kill process " << pid << "." << std::endl;
            }

        // Listar procesos activos
        } else if (command == "list") {
            pm.listProcesses();

        // Salir del programa
        } else if (command == "exit") {
            break;

        // Comando no reconocido
        } else {
            std::cout << "Unknown command. Please try again." << std::endl;
        }
    }
}