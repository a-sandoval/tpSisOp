#include "fileSystem/include/fileSystem.h"

int main(void) {


    logger = iniciarLogger("fileSys.log","file-system");

    config = iniciarConfiguracion("filesys.config"); 

    int conexionAMemoria = conexion("FILE-SYSTEM", "MEMORIA");
  
    escucharAlKernel(); 

    close(conexionAMemoria); 

    terminarPrograma(NULL);
    
    return 0;
}

void iterator(char *value) {
    free(value);
}






