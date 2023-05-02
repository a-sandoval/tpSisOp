#include "fileSystem/include/servidorKernel.h"

void escucharAlKernel() {
    char* puertoEscucha = confGet("PUERTO_ESCUCHA");
    socketCliente = alistarServidor(puertoEscucha);
    ejecutarServidor(socketCliente);  
}

