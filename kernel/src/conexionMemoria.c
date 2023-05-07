/* KERNEL- cliente | MEMORIA - sevidor*/
#include "kernel/include/conexionMemoria.h"

int conexionMemoria() {
     logger = cambiarNombre("Kernel-Memoria");
    int conexionAMemoria = conexion("MEMORIA");
    if(!(conexionAMemoria + 1))
        log_error(loggerError, "No se pudo conectar a memoria.");   
    else
        close(conexionAMemoria);

    return 0;
}
