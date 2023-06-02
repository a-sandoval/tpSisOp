/* KERNEL- cliente | MEMORIA - servidor*/
#include "kernel/include/conexionMemoria.h"

int conexionAMemoria;

void conexionMemoria() {
    
    logger = cambiarNombre(logger,"Kernel-Memoria");
    loggerError = cambiarNombre(loggerError, "Errores Kernel-Memoria");
    
    while(1) {
        conexionAMemoria = conexion("MEMORIA");

        if(conexionAMemoria != -1){
            return;
        }
        else {
            log_error(loggerError, "No se pudo conectar al servidor, socket %d, esperando 5 segundos y reintentando.", conexionAMemoria);
            sleep(5);
        }
    }
}


void recibirEstructurasInicialesMemoria(t_pcb* pcb) {

    t_paquete* paquete = crearPaquete(); 
    paquete->codigo_operacion = NEWPCB; 
    log_debug(logger, "PID <%d>: Se esta solicitando estructuras iniciales de memoria.", pcb->pid);
    enviarPaquete(paquete, conexionAMemoria); 
}

