/* KERNEL- cliente | FYLESYSTEM - sevidor*/
#include "kernel/include/conexiones/conexionFileSystem.h"

int conexionAFS; 

 void conexionFileSystem(){
    char * nombreAnterior = duplicarNombre (logger);
    logger = cambiarNombre(logger,"Kernel-FS");
    char * nombreAnteriorErrores = duplicarNombre (loggerError);
    loggerError = cambiarNombre(loggerError,"Errores Kernel-FS");
    pthread_t respuestaPeticionFS_h;

    conexionAFS = conexion("FILESYSTEM");
    logger = cambiarNombre(logger, nombreAnterior);
    loggerError = cambiarNombre(loggerError, nombreAnteriorErrores);

    if (!pthread_create(&respuestaPeticionFS_h, NULL, (void *) respuestaPeticionFS, NULL)) 
        pthread_detach(respuestaPeticionFS_h);
    else 
        {error ("Error al generar hilo para el planificador de largo plazo, terminando el programa.");}

    free (nombreAnterior);
    free (nombreAnteriorErrores);
    return;
}
    

