/* KERNEL- cliente | FYLESYSTEM - sevidor*/
#include "kernel/include/conexionFileSystem.h"

int conexionFileSystem(){
   logger = cambiarNombre("Kernel-FS");
    int conexionAFS = conexion("FILESYSTEM");
    if(!(conexionAFS + 1))
        log_error(logger, "No se pudo conectar al servidor.");
    else
        close(conexionAFS);

    return 0;
}
