/* KERNEL- cliente | MEMORIA - sevidor*/
#include "kernel/include/conexionMemoria.h"

int conexionAMemoria;

int conexionMemoria() {
    
    logger = cambiarNombre("Kernel-Memoria");
    
    while(1) {
        conexionAMemoria = conexion("MEMORIA");

        if(conexionAMemoria != -1)
            return;
        else {
            log_error(loggerError, "No se pudo conectar al servidor, socket %d, esperando 5 segundos y reintentando.", conexionAMemoria);
            sleep(5);
        }
    }
}

void recibirEstructurasIniciales() {


    enviarMensaje("Estructuras iniciales",conexionAMemoria); 

    

    
}

