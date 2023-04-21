#include "fileSystem/include/servidorKernel.h"

void escucharAlKernel() {

    char* puertoEscucha= confGet(config,"PUERTO_ESCUCHA"); 
    //log_info(logger, "Vamos bien por ahora"); 
    t_list* clavesValidas = list_create();

    list_add(clavesValidas,(
    void *) confGet(config,"CLAVE_FS_KERNEL_VALIDA"));

    int kernel_fd = alistarServidor(logger, puertoEscucha);

    //int resultadoEjecucion = 
    ejecutarServidor(kernel_fd, logger, clavesValidas);  

    list_destroy_and_destroy_elements(clavesValidas,element_destroyer);


}

