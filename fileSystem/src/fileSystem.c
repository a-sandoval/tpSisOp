#include "fileSystem/include/fileSystem.h"

int main(void) {
    logger = iniciarLogger("fileSys.log","file-system");
    config = iniciarConfiguracion("filesys.config");
    loggerError = iniciarLogger("errores.log", "File-System"); 
    socketMemoria = conexion("MEMORIA"); 
    escucharAlKernel();
    close(socketMemoria);
    terminarPrograma();
    return 0;
}

void iterator(void *value) {
    log_info(logger, "Se recibio esta data: %s; Se envia a la memoria.", (char *) value);
    t_paquete *paquete = crearPaquete();
    agregarAPaquete ( paquete, value, sizeof(char) * strlen((char *) value) + 1 );
    enviarPaquete(paquete, socketMemoria);
    eliminarPaquete(paquete);
}






