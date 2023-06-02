/* KERNEL- cliente | FYLESYSTEM - sevidor*/
#include "kernel/include/conexionFileSystem.h"

int conexionAFS; 

int conexionFileSystem(){
   logger = cambiarNombre(logger,"Kernel-FS");
   loggerError = cambiarNombre(loggerError,"Errores Kernel-FS");
    
    log_info(logger,"Conexion lista entre Kernel y File System"); 

    while(1) {
       
        conexionAFS = conexion("FILESYSTEM");

        if(conexionAFS != -1)
            return 0;
        else {
            log_error(loggerError, "No se pudo conectar al servidor, socket %d, esperando 5 segundos y reintentando.", conexionAFS);
            sleep(5);
        }
    }
}
    

