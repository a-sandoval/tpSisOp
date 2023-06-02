#include "kernel/include/syscalls.h"

t_list *recursos;
char **nombresRecursos;
char* segFault = "SEG_FAULT"; 
t_pcb* proceso;
estadoProceso estadoAnterior; 

void retornoContexto(t_pcb *pcb, t_contexto *contextoEjecucion){

    /*char *temp = string_duplicate(", parametros");

    for (int i = 0; i < contextoEjecucion->motivoDesalojo->parametrosLength; i++) 
        string_append_with_format(&temp, " %s ", contextoEjecucion->motivoDesalojo->parametros[i]);

    log_info (logger, "Contexto retornado con motivo: comando %d%s", contextoEjecucion->motivoDesalojo->comando, temp);

    free (temp); */ 

    proceso = pcb; 

    switch (contextoEjecucion->motivoDesalojo->comando){
        case IO:
            io_s(contextoEjecucion->motivoDesalojo->parametros);
            break;
        case WAIT:
            wait_s(contextoEjecucion->motivoDesalojo->parametros);
            break;
        case SIGNAL:
            signal_s(contextoEjecucion->motivoDesalojo->parametros);
            break;
        case YIELD:
            yield_s();
            break;
        case EXIT:
            exit_s(contextoEjecucion->motivoDesalojo->parametros);
            break;
    /*
        case F_OPEN:
            fopen_s(contextoEjecucion->motivoDesalojo->parametros);
            break;
        case F_CLOSE:
            fclose_s(proceso, contextoEjecucion->motivoDesalojo->parametros);
            break;
        case F_SEEK:
            fseek_s(contextoEjecucion->motivoDesalojo->parametros);
            break;
        case F_READ:
            fread_s(contextoEjecucion->motivoDesalojo->parametros);
            break;
        case F_WRITE:
            fwrite_s(contextoEjecucion->motivoDesalojo->parametros);
            break;
        case F_TRUNCATE:
            ftruncate_s(ontextoEjecucion->motivoDesalojo->parametros);
            break;
        case CREATE_SEGMENT:
            createSegment_s(contextoEjecucion->motivoDesalojo->parametros);
            break;
        case DELETE_SEGMENT:
            deleteSegment_s(contextoEjecucion->motivoDesalojo->parametros);
            break;
   
     */   
    default:
        log_error(loggerError, "Comando incorrecto");
        exit(1);
        break;
    }
}

void wait_s(char **parametros){

    char *recurso = parametros[0];

    int indexRecurso = indiceRecurso(recurso);

    if (indexRecurso == -1)
    {
        exit_s(&segFault); 
        return;
    }

    int instancRecurso = instanciasRecursos[indexRecurso];
    instancRecurso--;
    instanciasRecursos[indexRecurso]=instancRecurso;

    log_info(logger,"PID: <%d> - Wait: <%s> - Instancias: <%d>",proceso->pid,recurso,instancRecurso); 


    if(instancRecurso < 0){
        
        t_list *colaBloqueadosRecurso = (t_list *)list_get(recursos, indexRecurso);

        estadoAnterior = proceso->estado; 
        proceso->estado = BLOCK;

        list_add(colaBloqueadosRecurso, (void *)proceso);

        loggearCambioDeEstado(proceso->pid, estadoAnterior, proceso->estado);
        loggearBloqueoDeProcesos(recurso); 
        
    } 
   
    else 
        procesarPCB(proceso); 
}

void signal_s(char **parametros){

    char *recurso = parametros[0];

    int indexRecurso = indiceRecurso(recurso);

    if (indexRecurso == -1){
        exit_s(&segFault); 
        return;
    }

    int instancRecurso = instanciasRecursos[indexRecurso];

    instancRecurso++;

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
    
    procesarPCB(proceso); 
}

void io_s(char **parametros){   
    
    estadoAnterior = proceso->estado;
    proceso->estado = BLOCK; 

    loggearBloqueoDeProcesos("IO"); 
    loggearCambioDeEstado(proceso->pid, estadoAnterior, proceso->estado);
    
    int tiempo = atoi(parametros[0]);
    log_info(logger,"PID: <%d> - Ejecuta IO: <%d>",proceso->pid,tiempo); 
    bloqueoIO(tiempo); 
}


void bloqueoIO(int tiempo){

    pthread_t pcb_bloqueado;

    if (!pthread_create(&pcb_bloqueado, NULL, (void *)bloquearIO, (void *)&tiempo)){
        pthread_detach(pcb_bloqueado);
    } else {
        log_error(loggerError, "Error en la creacion de hilo para realizar I/O, Abort");
        abort();
    }
}

void bloquearIO(int tiempo){  

        sleep(tiempo); 
        estimacionNuevaRafaga(proceso); 
        estadoAnterior = proceso->estado;
        proceso->estado = READY;
        loggearCambioDeEstado(proceso->pid, estadoAnterior, proceso->estado);
        ingresarAReady(proceso);
}

void yield_s(){  

    estimacionNuevaRafaga(proceso); 
    estadoAnterior = proceso->estado;
    proceso->estado = READY;
    loggearCambioDeEstado(proceso->pid, estadoAnterior, proceso->estado);
    ingresarAReady(proceso); 
}

void exit_s(char **parametros){   
    
    estadoAnterior = proceso->estado; 
    proceso->estado = SALIDA; 

    loggearCambioDeEstado(proceso->pid, estadoAnterior, proceso->estado); 
    loggearSalidaDeProceso(parametros[0]); 
    
    enviarMensaje("Terminado", proceso->socketPCB);
    destruirPCB(proceso);
    sem_post(&semGradoMultiprogramacion); 
}

/*
void fopen_s(t_pcb *proceso, char **parametros)
{
}

void fclose_s(t_pcb *proceso, char **parametros)
{
}

void fseek_s(t_pcb *proceso, char **parametros)
{
}

void fread_s(t_pcb *proceso, char **parametros)
{
}

void fwrite_s(t_pcb *proceso, char **parametros)
{
}

void createSegment_s(t_pcb *proceso, char **parametros)
{
}

void deleteSegment_s(t_pcb *proceso, char **parametros)
{
}



*/

void loggearBloqueoDeProcesos(char* motivo) {
    log_info(logger,"PID: <%d> - Bloqueado por: %s", proceso->pid, motivo); 
}

void loggearSalidaDeProceso(char* motivo) {
    log_info(logger,"Finaliza el proceso <%d> - Motivo: <%s>", proceso->pid, motivo); 
}