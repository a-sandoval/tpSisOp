#include "servidorKernel.h"

void escucharAlKernel() {

    char* puertoEscucha;

    obtenerDeConfiguracionComoServidor(&puertoEscucha); 

    int kernel_fd = alistarServidor(logger, puertoEscucha);

    int resultadoEjecucion = ejecutarServidor(kernel_fd, logger);  

}


char* obtenerDeConfiguracionComoServidor(char ** puertoEscucha) {

    t_config* configServer = config_create("file-system.config");

    *puertoEscucha = config_get_string_value(configServer,"PUERTO_ESCUCHA");

    config_destroy(configServer); 

    return puertoEscucha; 
}
