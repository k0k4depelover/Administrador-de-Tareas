#Oskar


# Este archivo configura el proyecto AdministradorProcesos usando CMake 
# para compilar código C++ moderno (estándar C++20) en entornos multiplataforma 
# (Windows y Unix). Define cómo construir el ejecutable principal, especifica 
# los archivos fuente, incluye los directorios necesarios y ajusta el enlace con
# bibliotecas del sistema según el entorno.

cmake_minimum_required(VERSION 3.15)
project(AdministradorProcesos CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if(MINGW)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--subsystem,console")
endif()

#Donald
add_executable(AdministradorProcesos
    src/main.cpp
    src/core/listingProcess.cpp
    src/core/processCreate.cpp
    src/core/killProcess.cpp
)

target_include_directories(AdministradorProcesos PRIVATE
    ${Boost_INCLUDE_DIRS} 
    ${CMAKE_SOURCE_DIR}/src 
    ${CMAKE_SOURCE_DIR}/src/core)

if(WIN32)
    target_link_libraries(AdministradorProcesos PRIVATE psapi)
elseif(UNIX)
    add_definitions(-D_linux_)
endif()