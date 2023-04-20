#include "fileSystem/include/fileSystem.h"

int main(void) {


    logger = iniciarLogger("fileSys.log","file-system");

    config = iniciarConfiguracion("filesys.config", logger); 

    int conexionAMemoria = conectarseAMemoria(); 
  
    escucharAlKernel(); 

    liberar_conexion(conexionAMemoria); 

    log_destroy(logger); 

    config_destroy(config); 
    
    return 0;
}

void iterator(char *value) {
    free(value);
}






