#include "fileSystem/include/servidorKernel.h"

void escucharAlKernel() {

    char* puertoEscucha= confGet("PUERTO_ESCUCHA"); 

    t_list* clavesValidas = list_create();


    alistarServidor(puertoEscucha);

    ejecutarServidor(socketCliente, clavesValidas);  


}

