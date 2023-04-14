#include "../include/fileSystem.h"

int main(void) {


    logger = iniciar_logger("fileSys.log","file-system");

    t_config* config_fs = iniciarConfiguracion("filesys.config", logger); 

    int conexionAMemoria = conectarseAMemoria(config_fs); 
  
    escucharAlKernel(config_fs); 

    liberar_conexion(conexionAMemoria); 

    log_destroy(logger); 

    config_destroy(config_fs); 
    
    return 0;
}

void iterator(char *value) {
    free(value);
}






