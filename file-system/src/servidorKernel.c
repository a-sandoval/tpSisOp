#include "file-system/include/servidorKernel.h"

void escucharAlKernel() {

    char* puertoEscucha;
    char* claveValida; 

    Lista* lista = NULL; 

    obtenerDeConfiguracionComoServidor(&puertoEscucha, &claveValida); 

    insertar(lista,claveValida); 

    int kernel_fd = alistarServidor(logger, puertoEscucha);

    int resultadoEjecucion = ejecutarServidor(kernel_fd, logger, lista);  

}


void obtenerDeConfiguracionComoServidor(char ** puertoEscucha, char** claveValida) {

    t_config* configServer = config_create("file-system.config");

    *puertoEscucha = config_get_string_value(configServer,"PUERTO_ESCUCHA");

    *claveValida = config_get_string_value(configServer,"CLAVE_FS_KERNEL_VALIDA");

    config_destroy(configServer); 

}
