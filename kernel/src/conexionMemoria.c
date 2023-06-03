/* KERNEL- cliente | MEMORIA - servidor*/
#include "kernel/include/conexionMemoria.h"

int conexionAMemoria;

void conexionMemoria() {
    char * nombreAnterior = duplicarNombre (logger);
    logger = cambiarNombre (logger,"Kernel-Memoria");
    char * nombreAnteriorErrores = duplicarNombre (loggerError);
    loggerError = cambiarNombre (loggerError, "Errores Kernel-Memoria");
    
    while(1) {

        conexionAMemoria = conexion("MEMORIA");

        if(conexionAMemoria != -1){
            log_info(logger, "Conectado a Memoria");
            logger = cambiarNombre(logger, nombreAnterior);
            loggerError = cambiarNombre(loggerError, nombreAnteriorErrores);
            return;
        }
        else {
            log_error(loggerError, "No se pudo conectar a memoria, esperando 5 segundos y reintentando.");
            sleep(5);
        }
    }
}

void recibirEstructurasInicialesMemoria(t_pcb* pcb) {
    
    char * nombreAnterior = duplicarNombre(logger);
    logger = cambiarNombre(logger,"Kernel-Memoria");
    //t_paquete* paquete = crearPaquete(); 
    //paquete->codigo_operacion = NEWPCB; 
    log_debug(logger, "PID <%d>: Se esta solicitando estructuras iniciales de memoria.", pcb->pid);
    //enviarPaquete(paquete, conexionAMemoria); 
    enviarCodOp(NEWPCB, conexionAMemoria);
    logger = cambiarNombre(logger, nombreAnterior);
}

void liberarMemoriaPCB(t_pcb* proceso) {

    /*t_paquete* paquete = crearPaquete(); 
    paquete->codigo_operacion=ENDPCB; 

    enviarPaquete(paquete,conexionAMemoria); */
    enviarCodOp (ENDPCB, conexionAMemoria);
}