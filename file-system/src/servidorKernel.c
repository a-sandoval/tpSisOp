#include "servidorKernel.h"

void escucharAlKernel() {

    char* puertoEscucha;

    obtenerDeConfiguracionComoServidor(&puertoEscucha); 

    int kernel_fd = alistarServidor(logger, puertoEscucha); 

    //int fsAsServerFD = iniciar_servidor(puertoDeEscucha); 


   // int kernelAsClientFD = esperar_cliente(fsAsServerFD);

}


char* obtenerDeConfiguracionComoServidor(char ** puertoEscucha) {

    t_config* configServer = config_create("file-system.config");

    *puertoEscucha = config_get_string_value(configServer,"PUERTO_ESCUCHA");

    config_destroy(configServer); 

    return puertoEscucha; 
}
