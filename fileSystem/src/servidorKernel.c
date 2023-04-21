#include "fileSystem/include/servidorKernel.h"

void escucharAlKernel() {

    char* puertoEscucha= confGet("PUERTO_ESCUCHA"); 
    //log_info(logger, "Vamos bien por ahora"); 
    t_list* clavesValidas = list_create();

    list_add(clavesValidas,(void *) confGet("CLAVE_FS_KERNEL_VALIDA"));

    int kernel_fd = alistarServidor(puertoEscucha);

    //int resultadoEjecucion = 
    ejecutarServidor(kernel_fd, clavesValidas);  

    list_destroy(clavesValidas);

}

