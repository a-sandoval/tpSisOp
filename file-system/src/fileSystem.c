#include "../include/fileSystem.h"

int main(void) {


    logger = iniciar_logger("fileSys.log","file-system");

    log_info(logger, "Hola"); 
    t_config* config_fs = iniciarConfiguracion("/home/utnso/tp-2023-1c-toTheEnd/file-system/file-system.config", logger); 


    escucharAlKernel(); 

    int conexionAMemoria = conectarseAMemoria(); 

    log_info(logger,"Se han creado conexiones pertinentes"); // Luego comentar

    log_destroy(logger); 

    config_destroy(config_fs); 
    
    return 0;
}

void iterator(char *value) {
    free(value);
}




