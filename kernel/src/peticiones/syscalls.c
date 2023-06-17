#include "kernel/include/peticiones/syscalls.h"

t_list *recursos;
char **nombresRecursos;
char* segFault = "SEG_FAULT"; 
char* outOfMemory = "OUT_OF_MEMORY";
estadoProceso estadoAnterior; 

void retornoContexto(t_pcb *proceso, t_contexto *contextoEjecucion){
    switch (contextoEjecucion->motivoDesalojo->comando){
        case IO:
            io_s(proceso, contextoEjecucion->motivoDesalojo->parametros);
            break;
        case WAIT:
            wait_s(proceso, contextoEjecucion->motivoDesalojo->parametros);
            break;
        case SIGNAL:
            signal_s(proceso, contextoEjecucion->motivoDesalojo->parametros);
            break;
        case YIELD:
            yield_s(proceso);
            break;
        case EXIT:
            exit_s(proceso, contextoEjecucion->motivoDesalojo->parametros);
            break;
    /*
        case F_OPEN:
            fopen_s(proceso, contextoEjecucion->motivoDesalojo->parametros);
            break;
        case F_CLOSE:
            fclose_s(proceso, contextoEjecucion->motivoDesalojo->parametros);
            break;
        case F_SEEK:
            fseek_s(proceso, contextoEjecucion->motivoDesalojo->parametros);
            break;
        case F_READ:
            fread_s(proceso, contextoEjecucion->motivoDesalojo->parametros);
            break;
        case F_WRITE:
            fwrite_s(proceso, contextoEjecucion->motivoDesalojo->parametros);
            break;
        case F_TRUNCATE:
            ftruncate_s(proceso, contextoEjecucion->motivoDesalojo->parametros);
            break;
        case CREATE_SEGMENT:
            createSegment_s(proceso, contextoEjecucion->motivoDesalojo->parametros);
            break;
        case DELETE_SEGMENT:
            deleteSegment_s(proceso, contextoEjecucion->motivoDesalojo->parametros);
            break;
   
     */   
    default:
        log_error(loggerError, "Comando incorrecto, ejecutando Yield para continuar");
        yield_s(proceso);
        //exit(1);
        break;
    }
}

void volverACPU(t_pcb* proceso) {
    contextoEjecucion = procesarPCB(proceso);
    rafagaCPU = contextoEjecucion->rafagaCPUEjecutada; 
    retornoContexto(proceso, contextoEjecucion); 
    
}

void wait_s(t_pcb *proceso, char **parametros){

    char *recurso = parametros[0];

    int indexRecurso = indiceRecurso(recurso);

    if (indexRecurso == -1)
    {
        exit_s(proceso, &segFault); 
        return;
    }

    int instancRecurso = instanciasRecursos[indexRecurso];
    instancRecurso--;
    instanciasRecursos[indexRecurso]=instancRecurso;
    list_add(proceso->recursosAsignados, recurso);

    log_info(logger,"PID: <%d> - Wait: <%s> - Instancias: <%d>",proceso->pid,recurso,instancRecurso); 


    if(instancRecurso < 0){
        
        t_list *colaBloqueadosRecurso = (t_list *)list_get(recursos, indexRecurso);

        estadoAnterior = proceso->estado; 
        proceso->estado = BLOCK;

        list_add(colaBloqueadosRecurso, (void *)proceso);

        loggearCambioDeEstado(proceso->pid, estadoAnterior, proceso->estado);
        loggearBloqueoDeProcesos(proceso, recurso); 
        
    } 
    else {
        volverACPU(proceso);
    }
}

void signal_s(t_pcb *proceso, char **parametros){

    char *recurso = parametros[0];

    int indexRecurso = indiceRecurso(recurso);

    if (indexRecurso == -1){
        exit_s(proceso, &segFault); 
        return;
    }

    int instancRecurso = instanciasRecursos[indexRecurso];
    instancRecurso++;
    list_remove_element(proceso->recursosAsignados,(void*)recurso);

    log_info(logger,"PID: <%d> - Signal: <%s> - Instancias: <%d>",proceso->pid,recurso,instancRecurso); 

    instanciasRecursos[indexRecurso]=instancRecurso;

    if(instancRecurso <= 0){
        
        t_list *colaBloqueadosRecurso = (t_list *)list_get(recursos, indexRecurso);

        t_pcb* pcbDesbloqueado = desencolar(colaBloqueadosRecurso);

        estimacionNuevaRafaga(pcbDesbloqueado); 

        estadoAnterior = pcbDesbloqueado->estado;
        pcbDesbloqueado->estado = READY;

        loggearCambioDeEstado(pcbDesbloqueado->pid,estadoAnterior,pcbDesbloqueado->estado); 
        ingresarAReady(pcbDesbloqueado); 
    
    }
    
    volverACPU(proceso);    
}


void io_s(t_pcb *proceso, char **parametros){   
    
    estadoAnterior = proceso->estado;
    proceso->estado = BLOCK; 

    loggearBloqueoDeProcesos(proceso, "IO"); 
    loggearCambioDeEstado(proceso->pid, estadoAnterior, proceso->estado);
    
    int tiempo = atoi(parametros[0]);
    log_info(logger,"PID: <%d> - Ejecuta IO: <%d>",proceso->pid,tiempo); 
  
     pthread_t pcb_bloqueado;

    if (!pthread_create(&pcb_bloqueado, NULL, (void *)bloquearIO, (void *)&tiempo)){
        pthread_join(pcb_bloqueado,NULL);
        estimacionNuevaRafaga(proceso); 
        estadoAnterior = proceso->estado;
        proceso->estado = READY;
        loggearCambioDeEstado(proceso->pid, estadoAnterior, proceso->estado);
        ingresarAReady(proceso);
    } else {
        log_error(loggerError, "Error en la creacion de hilo para realizar I/O, Abort");
        abort();
    } 
     
}

void bloquearIO(int *tiempo){  
       
    sleep(*tiempo);
        
}

void yield_s(t_pcb *proceso){  

    estimacionNuevaRafaga(proceso); 
    estadoAnterior = proceso->estado;
    proceso->estado = READY;
    loggearCambioDeEstado(proceso->pid, estadoAnterior, proceso->estado);
    ingresarAReady(proceso); 
}

void exit_s(t_pcb* proceso, char **parametros){   
    
    estadoAnterior = proceso->estado; 
    proceso->estado = SALIDA; 

    loggearCambioDeEstado(proceso->pid, estadoAnterior, proceso->estado); 
    loggearSalidaDeProceso(proceso, parametros[0]); 
    
    enviarMensaje("Terminado", proceso->socketPCB);

    liberarRecursosAsignados(proceso);
    liberarArchivosAsignados(proceso);
    liberarSegmentos(proceso);
    liberarMemoriaPCB(proceso); 

    destruirPCB(proceso); 
    sem_post(&semGradoMultiprogramacion); 
}

void liberarRecursosAsignados(t_pcb* proceso){

     int cantRecursos = list_size(proceso->recursosAsignados);

    uint32_t i;
    for(i=0; i<cantRecursos;i++){
        signal_s(proceso, list_get(proceso->recursosAsignados, i));
    }
}


void fopen_s(t_pcb *proceso, char **parametros){

    char* archivo = parametros[0];
    //primero veo si esta en la tabla global
    if(estaEnLaTablaGlobal(archivo)){

    }
    else{
        solicitarArchivoFS(archivo);
    }
    
}


/*
void fclose_s(t_pcb *proceso, char **parametros){

     log_info(logger, "PID: <PID> - Cerrar Archivo: <NOMBRE ARCHIVO>",);
    
}

void ftruncate_s(t_pcb *proceso, char **parametros){

     log_info(logger, "PID: <PID> - Archivo: <NOMBRE ARCHIVO> - Tamaño: <TAMAÑO>",);
    
}

void fseek_s(t_pcb *proceso, char **parametros){

    log_info(logger, "PID: <PID> - Actualizar puntero Archivo: <NOMBRE ARCHIVO> - Puntero <PUNTERO>",);
}

void fread_s(t_pcb *proceso, char **parametros){

    log_info(logger, "PID: <PID> - Leer Archivo: <NOMBRE ARCHIVO> - Puntero <PUNTERO> - Dirección Memoria <DIRECCIÓN MEMORIA> - Tamaño <TAMAÑO>",);
}

void fwrite_s(t_pcb *proceso, char **parametros){

    log_info(logger, "PID: <PID> - Escribir Archivo: <NOMBRE ARCHIVO> - Puntero <PUNTERO> - Dirección Memoria <DIRECCIÓN MEMORIA> - Tamaño <TAMAÑO>"",);
}
*/

void createSegment_s(t_pcb *proceso, char **parametros){

    uint32_t idSegmento = parametros[0];
    int tamanio = parametros[1];

    
    t_paquete* peticion = crearPaquete();
    peticion->codigo_operacion = CREATE_SEGMENT_OP;

    agregarAPaquete(peticion, (void*)&contextoEjecucion->pid, sizeof(uint32_t));
    agregarAPaquete(peticion, (void*)&idSegmento, sizeof(uint32_t));
    agregarAPaquete(peticion, (void*)&tamanio, sizeof(uint32_t));

    enviarPaquete(peticion, conexionAMemoria);
    
    t_paquete* rdoPeticion = crearPaquete();
    rdoPeticion = recibirPaquete(conexionAMemoria);
    // aca deberia venir en el opcode el rdo de lo que paso en memoria para saber como seguir

    switch(rdoPeticion->codigo_operacion){
        case SUCCESS:
                log_info(logger, "PID: %d - Crear Segmento - Id: %d - Tamanio: %d", proceso->pid, idSegmento, tamanio);
                // me mandan la tabla con el nuevo segmento incorporado
                recibirTablaActualizada();
                contextoEjecucion = procesarPCB(proceso);
                rafagaCPU = contextoEjecucion->rafagaCPUEjecutada; 
                retornoContexto(proceso, contextoEjecucion);
                break;

        case OUTOFMEMORY:
                exit_s(proceso, &outOfMemory);
                break;
        
        case COMPACTACION:
                log_info(logger, "Compactacion: Se solicito compactacion ");
                log_info(logger,  "Compactacion: Esperando Fin de Operaciones de FS");
                log_info(logger,  "Se finalizo el proceso de compactacion");

                //dsps de la compactacion
                 log_info(logger, "PID: %d - Crear Segmento - Id: %d - Tamanio: %d", proceso->pid, idSegmento, tamanio);
                break;
    }

    free(peticion);
    free(rdoPeticion);
}


void deleteSegment_s(t_pcb *proceso, char **parametros){
    
    uint32_t idSegmento = parametros[0];

    log_info(logger, "PID: %d - Eliminar Segmento - Id: %d", proceso->pid, idSegmento);

    t_paquete* peticion = crearPaquete();
    peticion->codigo_operacion = DELETE_SEGMENT_OP;

    agregarAPaquete(peticion, (void*)&contextoEjecucion->pid, sizeof(uint32_t));
    agregarAPaquete(peticion, (void*)&idSegmento, sizeof(int));
    enviarPaquete(peticion, conexionAMemoria);

    recibirTablaActualizada();

    contextoEjecucion = procesarPCB(proceso);
    rafagaCPU = contextoEjecucion->rafagaCPUEjecutada; 
    retornoContexto(proceso, contextoEjecucion);

}

void recibirTablaActualizada(){

	int size, desplazamiento = 0;
	void * buffer;

	buffer = recibirBuffer(conexionAMemoria, &size);

    desplazamiento += sizeof(int);
    memcpy(&(contextoEjecucion->pid), buffer + desplazamiento, sizeof(uint32_t));
    desplazamiento += sizeof(uint32_t);
    
    t_segmento* segmentoAux = malloc(sizeof(t_segmento));

    list_clean_and_destroy_elements (contextoEjecucion->tablaDeSegmentos, free);
    
    memcpy(&(contextoEjecucion->tablaDeSegmentosSize), buffer + desplazamiento, sizeof(uint32_t));
    desplazamiento += sizeof(uint32_t);
    
    for (uint32_t i = 0; i < contextoEjecucion->tablaDeSegmentos; i++) {

        memcpy (&segmentoAux->id, buffer + desplazamiento, sizeof (uint32_t));
        desplazamiento += sizeof (uint32_t);
        memcpy (&segmentoAux->direccionBase, buffer + desplazamiento, sizeof (uint32_t));
        desplazamiento += sizeof (uint32_t);
        memcpy (&segmentoAux->tamanio, buffer + desplazamiento, sizeof (uint32_t));
        desplazamiento += sizeof (uint32_t);
        
        list_add (contextoEjecucion->tablaDeSegmentos, segmentoAux);
        free (segmentoAux);
    }
    
}


void loggearBloqueoDeProcesos(t_pcb* proceso, char* motivo) {
    log_info(logger,"PID: <%d> - Bloqueado por: %s", proceso->pid, motivo); 
}

void loggearSalidaDeProceso(t_pcb* proceso, char* motivo) {
    log_info(logger,"Finaliza el proceso <%d> - Motivo: <%s>", proceso->pid, motivo); 
}

