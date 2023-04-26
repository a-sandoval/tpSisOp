#include "fileSystem/include/fileSystem.h"

int main(void) {


    logger = iniciarLogger("fileSys.log","file-system");

    config = iniciarConfiguracion("filesys.config"); 

    int conexionAMemoria = conectarseAMemoria(); 
  
    escucharAlKernel(); 

    close(conexionAMemoria); 

    terminarPrograma();
    
    return 0;
}

void iterator(void*value) {
    free(value);
}






