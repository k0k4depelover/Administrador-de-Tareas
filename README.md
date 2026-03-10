# 🖥️ Administrador de Tareas

**Administrador de Tareas** es un gestor de procesos de sistema operativo multiplataforma implementado en C++, desarrollado para el curso de **Sistemas Operativos I**. Permite ejecutar, monitorear y terminar procesos directamente desde CLI usando las llamadas al sistema nativas de cada plataforma.

---

## ✨ Características

- ⚡ **Multiplataforma** — Funciona en Linux/macOS (POSIX) y Windows (Win32 API) con el mismo código fuente.
- 🔄 **Ciclo de vida completo** — Crea procesos hijos, los lista y los termina por PID.
- 🧱 **OOP** — Clase `ProcessManager` encapsula toda la lógica del OS.
- 🛠️ **CMake Build** — Sistema de compilación portable.

---

## 🛠️ Stack Tecnológico

| Tecnología | Uso |
|---|---|
| C++ | Lenguaje principal |
| POSIX API (`fork`, `exec`, `kill`) | Gestión de procesos en Unix/Linux |
| Win32 API (`CreateProcess`, `TerminateProcess`) | Gestión en Windows |
| CMake | Sistema de compilación |

---

## 🏗️ Estructura

```
src/
├── main.cpp                # CLI principal (bucle de comandos)
└── core/
    ├── ProcessManager.h    # Interfaz pública del gestor
    ├── processCreate.cpp   # runProcess()
    ├── killProcess.cpp     # killProcess()
    └── listingProcess.cpp  # listProcesses()
CMakeLists.txt
```

---

## 💻 Ejemplo de Uso

```
------------------------------------------
Enter command (run, kill, list, exit): run
Enter command to run: firefox
Process started with PID: 12453

------------------------------------------
Enter command (run, kill, list, exit): list
PID: 12453 | Command: firefox

------------------------------------------
Enter command (run, kill, list, exit): kill
Enter PID to kill: 12453
Process 12453 killed successfully.
```

### Abstracción multiplataforma del PID

```cpp
// El mismo código compila en Windows y Unix
#ifdef _WIN32
    #include <minwindef.h>
    typedef DWORD pid_type;    // Windows: DWORD de 32 bits
#else
    #include <unistd.h>
    typedef pid_t pid_type;    // Unix: pid_t (int con signo)
#endif
```

### Interfaz de ProcessManager

```cpp
class ProcessManager {
public:
    // Ejecuta un proceso desde un string — retorna PID o -1 si falla
    int runProcess(const std::string& cmd);

    // Termina un proceso por PID — retorna true si tuvo éxito
    bool killProcess(int pid);

    // Lista todos los procesos activos gestionados
    bool listProcesses();

    // Divide "firefox --new-window" en ["firefox", "--new-window"]
    std::vector<std::string> splitCommand(const std::string& cmd);
};
```

---

## 📦 Compilación

```bash
git clone https://github.com/k0k4depelover/Administrador-de-Tareas.git
cd Administrador-de-Tareas
mkdir build && cd build
cmake .. && cmake --build .
./Administrador-de-Tareas
```

---

## 📚 Conceptos de SO Aplicados

- **Procesos e Hilos** — Creación de procesos hijos con `fork/exec` (Unix) o `CreateProcess` (Windows).
- **Señales** — Terminación con `SIGKILL` vía `kill()` (Unix) o `TerminateProcess()` (Windows).
- **PCB** — El PID como identificador único de proceso en el kernel.
- **Syscalls** — Interacción directa con el kernel para operaciones de proceso.

---

## 📄 Licencia

MIT