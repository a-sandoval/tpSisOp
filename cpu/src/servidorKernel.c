#include "cpu/include/servidorKernel.h"

void escucharAlKernel() {

    char* puertoEscucha= confGet("PUERTO_ESCUCHA"); 

    t_list* clavesValidas = list_create();

    list_add(clavesValidas,(void *) confGet("CLAVE_CPU_KERNEL"));

    alistarServidor(puertoEscucha);

    ejecutarServidor(socketCliente, clavesValidas);  

    list_destroy(clavesValidas);

}

