// Oskar
// Encabezado de la clase ProcessManager, que gestiona procesos del sistema (ejecutar, terminar, listar, etc.)

#ifndef PROCESSMANAGER_H
#define PROCESSMANAGER_H

#include <string>   // Para manejar comandos como cadenas de texto
#include <vector>   // Para devolver listas de argumentos o procesos

// Clase que encapsula operaciones relacionadas con la gestión de procesos
class ProcessManager {
public:
    // Ejecuta un proceso a partir de un comando en forma de string.
    // Devuelve el PID del proceso si se inicia correctamente, o -1 si falla.
    int runProcess(const std::string& cmd);

    // Intenta terminar un proceso dado su PID.
    // Devuelve true si se logra matar el proceso, false si falla.
    bool killProcess(int pid);

    // Divide un comando en sus argumentos individuales.
    // Útil para pasar argumentos al sistema operativo.
    std::vector<std::string> splitCommand(const std::string& cmd);

    // Lista los procesos activos gestionados por esta clase.
    // Devuelve true si la operación fue exitosa.
    bool listProcesses();

    // Verifica si el comando es corto (por ejemplo, sin argumentos).
    // Puede usarse para validar o categorizar comandos.
    bool isShortCommand(const std::string& cmd);
};

#endif // PROCESSMANAGER_HW