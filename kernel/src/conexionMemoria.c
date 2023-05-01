/* KERNEL- cliente | MEMORIA - sevidor*/
#include "kernel/include/conexionMemoria.h"

int conexionMemoria() {
     logger = cambiarNombre("Kernel-Memoria");
    int conexionAMemoria = conexion("MEMORIA");
    if(!(conexionAMemoria + 1))
        log_error(logger, "No se pudo conectar al servidor.");   
    else
        close(conexionAMemoria);

    return 0;
}
