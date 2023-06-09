/* KERNEL- cliente | MEMORIA - servidor*/
#include "kernel/include/conexiones/conexionMemoria.h"

int conexionAMemoria;

void conexionMemoria() {
    char * nombreAnterior = duplicarNombre (logger);
    logger = cambiarNombre (logger,"Kernel-Memoria");
    char * nombreAnteriorErrores = duplicarNombre (loggerError);
    loggerError = cambiarNombre (loggerError, "Errores Kernel-Memoria");
    
    

    conexionAMemoria = conexion("MEMORIA");
    logger = cambiarNombre(logger, nombreAnterior);
    loggerError = cambiarNombre(loggerError, nombreAnteriorErrores);
    free (nombreAnterior);
    free (nombreAnteriorErrores);
    return;
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
    free (nombreAnterior);

}

void liberarMemoriaPCB(t_pcb* proceso) {
    
    char * nombreAnterior = duplicarNombre(logger);
    logger = cambiarNombre(logger,"Kernel-Memoria");

    /*t_paquete* paquete = crearPaquete(); 
    paquete->codigo_operacion=ENDPCB; 

    enviarPaquete(paquete,conexionAMemoria); */
    log_info(logger, "PID <%d>: Se envia señal para eliminar estructuras en memoria.", proceso->pid);
    enviarCodOp (ENDPCB, conexionAMemoria);
    logger = cambiarNombre(logger, nombreAnterior);
    free (nombreAnterior);
}