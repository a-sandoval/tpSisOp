/* KERNEL- cliente | FYLESYSTEM - sevidor*/
#include "kernel/include/conexionFileSystem.h"

int conexionAFS; 

 void conexionFileSystem(){
    char * nombreAnterior = duplicarNombre (logger);
    logger = cambiarNombre(logger,"Kernel-FS");
    char * nombreAnteriorErrores = duplicarNombre (loggerError);
    loggerError = cambiarNombre(loggerError,"Errores Kernel-FS");

    while(1) {
       
        conexionAFS = conexion("FILESYSTEM");

        if(conexionAFS != -1) {
            log_info(logger, "Conectado a File System");
            logger = cambiarNombre(logger, nombreAnterior);
            loggerError = cambiarNombre(loggerError, nombreAnteriorErrores);
            return;
        }
        else {
            log_error(loggerError, "No se pudo conectar al File System, esperando 5 segundos y reintentando.");
            sleep(5);
        }
    }
}
    

