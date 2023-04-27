/* - Interprete entre el hardware y el software - */
#include "kernel/include/kernel.h"

int main(){


    /*Inicializando Logger*/
    logger = iniciarLogger("kernel.log", "Kernel");

    /*Inicializando el config*/
    config = iniciarConfiguracion("kernel.config");
    
    /*
    // Generar conexion a memoria
    logger = cambiarNombre("Kernel-Memoria");
    int conexionAMemoria = conexion("KERNEL", "MEMORIA");
    if(!(conexionAMemoria + 1))
        log_error(logger, "No se pudo conectar al servidor.");   
    else
        close(conexionAMemoria);

    // Generar conexion a CPU    
    logger = cambiarNombre("Kernel-CPU");
    int conexionACPU = conexion("KERNEL", "CPU");
    if(!(conexionACPU + 1))
        log_error(logger, "No se pudo conectar al servidor.");
    else
        close(conexionACPU);

    // Generar conexion a File System
    logger = cambiarNombre("Kernel-FS");
    int conexionAFS = conexion("KERNEL", "FILESYSTEM");
    if(!(conexionAFS + 1))
        log_error(logger, "No se pudo conectar al servidor.");
    else
        close(conexionAFS);

    */

    logger = cambiarNombre("Kernel-Consola");
    int servidorDeConsola = servirAConsola();
    close(servidorDeConsola);

    terminarPrograma();

    return 0; 
}