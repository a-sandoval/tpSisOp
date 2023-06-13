#include "fileSystem/include/conexionMemoria.h"

int conexionAMemoria; 

void conexionMemoria() {

    char * nombreAnterior = duplicarNombre (logger);
    logger = cambiarNombre(logger, "File System - Memoria");
    char * nombreAnteriorErrores = duplicarNombre (loggerError);
    loggerError = cambiarNombre(loggerError,"Errores File System - Memoria");
    conexionAMemoria = conexion("MEMORIA");
    logger = cambiarNombre(logger, nombreAnterior);
    loggerError = cambiarNombre(loggerError, nombreAnteriorErrores);
    return;
}