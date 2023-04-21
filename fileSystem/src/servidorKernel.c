#include "fileSystem/include/servidorKernel.h"

void escucharAlKernel() {

    char* puertoEscucha= confGet(config,"PUERTO_ESCUCHA"); 
    char* claveValida = confGet(config,"CLAVE_FS_KERNEL_VALIDA");

    //log_info(logger, "Vamos bien por ahora"); 
    Lista* listaClavesValidas = malloc(sizeof(Lista)); 
    listaClavesValidas-> cabeza = NULL; 

    insertar(listaClavesValidas,claveValida); 

    int kernel_fd = alistarServidor(logger, puertoEscucha);

    //int resultadoEjecucion = 
    ejecutarServidor(kernel_fd, logger, listaClavesValidas);  

    borrarLista(listaClavesValidas); 


}

