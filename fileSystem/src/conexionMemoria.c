#include "fileSystem/include/conexionMemoria.h"

int conexionAMemoria; 

void conexionMemoria() {

    char * nombreAnterior = duplicarNombre (logger);
    logger = cambiarNombre(logger, "File-System - Memoria");
    char * nombreAnteriorErrores = duplicarNombre (loggerError);
    loggerError = cambiarNombre(loggerError,"Errores FS- Memoria");

    while(1) {
        conexionAMemoria = conexion("MEMORIA");

        if(conexionAMemoria != -1){
            log_info(logger, "Conectado a Memoria");
            logger = cambiarNombre(logger, nombreAnterior);
            loggerError = cambiarNombre(loggerError, nombreAnteriorErrores);
            return;
        }
        else {
            log_error(loggerError, "No se pudo conectar al servidor, socket %d, esperando 5 segundos y reintentando.", conexionAMemoria);
            sleep(5);
        }
    }
}