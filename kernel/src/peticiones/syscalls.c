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

    t_archivo* archivo = malloc(sizeof(t_archivo));
    char* nombreArchivo = parametros[0];
    //primero veo si esta en la tabla global
    if(estaEnLaTablaGlobal(nombreArchivo)){
            // si esta en la tabla alguien ya lo esta usando y tengo que ponerlo en block
            estadoAnterior = proceso->estado;
            proceso->estado = BLOCK;
            
            //noc si este list find funciona
            archivo = list_find(tablaGlobalArchivos, estaEnLaTablaGlobal(nombreArchivo));

            list_add(archivo->colaBloqueados, proceso);
            archivo->colaBloqueadosSize++;
            loggearCambioDeEstado(proceso->pid, estadoAnterior, proceso->estado);
            loggearBloqueoDeProcesos(proceso, nombreArchivo);
    }

    else{
        archivo = solicitarArchivoFS(nombreArchivo);
        list_add(proceso->tablaDeArchivos,(void*)archivo);
        volverACPU(proceso);
    }
    free(archivo);
    
}

bool estaEnLaTablaGlobal(char* nombreArchivo){

    int cantArchivos = list_size(tablaGlobalArchivos);
    t_archivo* archivoAux = malloc(sizeof(t_archivo));


    for(int i=0; i<cantArchivos; i++){
        archivoAux=list_get(tablaGlobalArchivos, i);

        if(!strcmp(nombreArchivo, archivoAux->fcb->nombre)){
            free(archivoAux);
            return true;
        }
    }
    free(archivoAux);
    return false;
}


void fclose_s(t_pcb *proceso, char **parametros){

    char* nombreArchivo = parametros[0];

    log_info(logger, "PID: <PID> - Cerrar Archivo: <NOMBRE ARCHIVO>",proceso->pid, nombreArchivo);

    t_archivo* archivo = malloc(sizeof(t_archivo));

    //archivo = list_find(tablaArchivosGlobal);

    //lo saco de la tabla del proceso
    list_remove_element(proceso->tablaDeArchivos, archivo->fcb);

    
    //SI ES 0, lo saco de la tabla
    if(archivo->colaBloqueadosSize == 0){
        list_remove_element(tablaGlobalArchivos, archivo);
    }
    else{
        t_pcb* procesoDesbloqueo = crearPCB();
        procesoDesbloqueo = list_get(archivo->colaBloqueados, 0);
        list_remove(archivo->colaBloqueados, 0);
        archivo->colaBloqueadosSize--;

        estadoAnterior = proceso->estado;
        proceso->estado = READY;
        list_add(proceso->tablaDeArchivos, archivo->fcb);
        loggearCambioDeEstado(proceso->pid, estadoAnterior, proceso->estado);
        ingresarAReady(proceso);


    }

    
}

/*
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
    
    int rdoPeticion = recibirCodigoDeOperacion(conexionAMemoria);

    switch(rdoPeticion){
        case SUCCESS:
                log_info(logger, "PID: %d - Crear Segmento - Id: %d - Tamanio: %d", proceso->pid, idSegmento, tamanio);
                // me mandan la tabla con el nuevo segmento incorporado
                recibirTablaActualizada(proceso);
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

    recibirTablaActualizada(proceso);

    contextoEjecucion = procesarPCB(proceso);
    rafagaCPU = contextoEjecucion->rafagaCPUEjecutada; 
    retornoContexto(proceso, contextoEjecucion);

}

void recibirTablaActualizada(t_pcb* pcb){

	int size, desplazamiento = 0;
	void * buffer;

	buffer = recibirBuffer(conexionAMemoria, &size);

    desplazamiento += sizeof(int);
    //salteo el pid aca porque ya lo tengo del pcb
    desplazamiento += sizeof(uint32_t);
    
    t_segmento* segmentoAux = malloc(sizeof(t_segmento));

    list_clean_and_destroy_elements (contextoEjecucion->tablaDeSegmentos, free);
    
    int tamanio;
    int tablaDeSegmentosSize;
    // Desplazamiento: tamaño de la lista de segmentos.
    memcpy(&(tablaDeSegmentosSize), buffer + desplazamiento, sizeof(uint32_t));
    desplazamiento += sizeof(uint32_t);
    
    for (uint32_t i = 0; i < tablaDeSegmentosSize; i++) {

        // Desplazamiento: Tamaño del segmento.
        memcpy (&tamanio, buffer + desplazamiento, sizeof (int));
        desplazamiento += sizeof (int);

        t_segmento* segmento = deserializarSegmento(buffer, &desplazamiento);
        list_add (pcb->tablaDeSegmentos, segmento);
        free(segmento);
    }

    desplazamiento += sizeof(int);

}

void loggearBloqueoDeProcesos(t_pcb* proceso, char* motivo) {
    log_info(logger,"PID: <%d> - Bloqueado por: %s", proceso->pid, motivo); 
}

void loggearSalidaDeProceso(t_pcb* proceso, char* motivo) {
    log_info(logger,"Finaliza el proceso <%d> - Motivo: <%s>", proceso->pid, motivo); 
}

