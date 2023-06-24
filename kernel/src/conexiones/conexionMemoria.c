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
    
    t_paquete* peticion = crearPaquete(); 
    peticion->codigo_operacion = NEWPCB; 
    agregarAPaquete(peticion,(void*)&pcb->pid, sizeof(uint32_t));
    enviarPaquete(peticion, conexionAMemoria); 
    free (peticion);
    log_debug(logger, "PID <%d>: Se esta solicitando estructuras iniciales de memoria.", pcb->pid);
    int codOp = recibirOperacion (conexionAMemoria);
    if (codOp == TABLADESEGMENTOS) {
        int size, desplazamiento = 0;
        void * data = recibirBuffer (conexionAMemoria, &(size));
        uint32_t pid;
        desplazamiento += sizeof (int);
        memcpy (&(pid), data + desplazamiento, sizeof pid);
        desplazamiento += sizeof pid;
        while (desplazamiento < size) {
            desplazamiento += sizeof (int);
            t_segmento * segmento = malloc (sizeof (t_segmento));
            memcpy (&(segmento->id), data + desplazamiento, sizeof (uint32_t));
            desplazamiento += sizeof (int) + sizeof (uint32_t);
            memcpy (&(segmento->direccionBase), data + desplazamiento, sizeof (uint32_t));
            desplazamiento += sizeof (int) + sizeof (uint32_t);
            memcpy (&(segmento->tamanio), data + desplazamiento, sizeof (uint32_t));
            desplazamiento += sizeof (uint32_t);
            list_add (pcb->tablaDeSegmentos, segmento);
            log_debug (logger, "Añadido segmento %d, direccion %d, tamaño %d", segmento->id, segmento->direccionBase, segmento->tamanio);
        }
    }
    else log_debug (logger, "No se recibio la tabla.");
    logger = cambiarNombre(logger, nombreAnterior);
    free (nombreAnterior);
}


void liberarMemoriaPCB(t_pcb* proceso){
    
    char * nombreAnterior = duplicarNombre(logger);
    logger = cambiarNombre(logger,"Kernel-Memoria");

    log_info(logger, "PID <%d>: Se envia señal para eliminar estructuras en memoria.", proceso->pid);
    logger = cambiarNombre(logger, nombreAnterior);
    free (nombreAnterior);

    t_paquete* peticion = crearPaquete(); 
    peticion->codigo_operacion = ENDPCB; 
    agregarAPaquete(peticion,(void*)&proceso->pid, sizeof(uint32_t));
    enviarPaquete(peticion, conexionAMemoria); 
}

