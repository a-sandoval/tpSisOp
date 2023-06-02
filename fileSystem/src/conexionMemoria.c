#include "fileSystem/include/conexionMemoria.h"

int conexionAMemoria; 

int conexionMemoria() {

    logger = cambiarNombre(logger, "File-System - Memoria");
    loggerError = cambiarNombre(loggerError,"Errores FS- Memoria");

    while(1) {
        conexionAMemoria = conexion("MEMORIA");

        if(conexionAMemoria != -1){
            enviarMensaje("Hola Memoria", conexionAMemoria); 
            return 0;
        }
        else {
            log_error(loggerError, "No se pudo conectar al servidor, socket %d, esperando 5 segundos y reintentando.", conexionAMemoria);
            sleep(5);
        }
    }
}