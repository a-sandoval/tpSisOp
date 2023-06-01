#include "kernel/include/syscalls.h"

t_list *recursos;
char **nombresRecursos;
char* segFault = "SEG_FAULT"; 

void retornoContexto(t_pcb *proceso, t_contexto *contextoEjecucion){

    char *temp = string_duplicate(", parametros");

    for (int i = 0; i < contextoEjecucion->motivoDesalojo->parametrosLength; i++) 
        string_append_with_format(&temp, " %s ", contextoEjecucion->motivoDesalojo->parametros[i]);

    log_info (logger, "Contexto retornado con motivo: comando %d%s", contextoEjecucion->motivoDesalojo->comando, temp);

    free (temp);

    switch (contextoEjecucion->motivoDesalojo->comando)
    {
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
        yield_s(proceso, contextoEjecucion->motivoDesalojo->parametros);
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
        estadoProceso anterior = proceso->estado;
        proceso->estado = READY;
        ingresarAReady(proceso);
        loggearCambioDeEstado(proceso->pid, anterior, proceso->estado);
        break;
    }
}

void wait_s(t_pcb *proceso, char **parametros)
{

    char *recurso = parametros[0];

    int indexRecurso = indiceRecurso(recurso);
    //printf("Indice recurso: %d \n",indexRecurso);

    if (indexRecurso == -1)
    {
        exit_s(proceso,&segFault); 
    }

    int instancRecurso = instanciasRecursos[indexRecurso];

    //printf("Instancia actual del recurso: %d \n",instancRecurso);

    instancRecurso--;

    instanciasRecursos[indexRecurso]=instancRecurso;

    log_info(logger,"PID: <%d> - Wait: <%s> - Instancias: <%d>",proceso->pid,recurso,instancRecurso); 

    //printf("Instancia actualizada: %d \n",instanciasRecursos[indexRecurso]);

    if (instancRecurso < 0)
    {
        t_list *colaBloqueadosRecurso = (t_list *)list_get(recursos, indexRecurso);

        //imprimirMatrizRecursosColaBloqueados(recursos,cantidadRecursos);

        loggearBloqueoDeProcesos(proceso,recurso); 

        list_add(colaBloqueadosRecurso, (void *)proceso);

        //imprimirMatrizRecursosColaBloqueados(recursos,cantidadRecursos);

        proceso->estado = BLOCK;
    }
    else {
        estadoProceso anterior = proceso->estado;
        proceso->estado = READY;
        loggearCambioDeEstado(proceso->pid, anterior, proceso->estado);
        ingresarAReady(proceso);
    }
}

void signal_s(t_pcb *proceso, char **parametros)
{
	//printf("SIGNAL \n \n");

    char *recurso = parametros[0];

    int indexRecurso = indiceRecurso(recurso);

    //printf("Indice recurso: %d \n",indexRecurso);

    if (indexRecurso == -1)
    {
        exit_s(proceso,&segFault); 

    }

    int instancRecurso = instanciasRecursos[indexRecurso];

    //printf("Instancia actual del recurso: %d \n",instancRecurso);

    instancRecurso++;

    log_info(logger,"PID: <%d> - Signal: <%s> - Instancias: <%d>",proceso->pid,recurso,instancRecurso); 

    instanciasRecursos[indexRecurso]=instancRecurso;

    //printf("Instancia actualizada: %d \n",instanciasRecursos[indexRecurso]);


    if (instancRecurso <= 0)
    {
        estadoProceso anterior = proceso->estado;
        proceso->estado = READY;
        loggearCambioDeEstado(proceso->pid, anterior, proceso->estado);
        ingresarAReady(proceso);

        // Se devuelve el proceso primero para seguir operando

        t_list *colaBloqueadosRecurso = (t_list *)list_get(recursos, indexRecurso);

        //printf("Antes de sacar un desbloqueado \n");
        //imprimirMatrizRecursosColaBloqueados(recursos,cantidadRecursos);

        t_pcb* pcbDesbloqueado = desencolar(colaBloqueadosRecurso);

        estimacionNuevaRafaga(pcbDesbloqueado); 

        estadoProceso estadoAnterior = pcbDesbloqueado->estado;
        pcbDesbloqueado->estado = READY;

        loggearCambioDeEstado(pcbDesbloqueado->pid,estadoAnterior,pcbDesbloqueado->estado); 
        ingresarAReady(pcbDesbloqueado); 
        //printf("Se puso en ready el pcb de pid: %d\n",pcbDesbloqueado->pid);

        //printf("Despues de sacar un desbloqueado \n");
        //imprimirMatrizRecursosColaBloqueados(recursos,cantidadRecursos);
        // Porque estaba procesarPCB aca?????????
    }
    else {
        estadoProceso anterior = proceso->estado;
        proceso->estado = READY;
        loggearCambioDeEstado(proceso->pid, anterior, proceso->estado);
        ingresarAReady(proceso);
    }

   
}

void io_s(t_pcb *proceso, char **parametros)
{   
    estadoProceso anterior = proceso->estado;
    proceso->estado = BLOCK; 

    loggearBloqueoDeProcesos(proceso,"IO"); 
    loggearCambioDeEstado(proceso->pid, anterior, proceso->estado);
    

    int tiempo = atoi(parametros[0]);
    log_info(logger,"PID: <%d> - Ejecuta IO: <%d>",proceso->pid,tiempo); 
    bloqueoIO(tiempo);

    estimacionNuevaRafaga(proceso); 

    anterior = proceso->estado;
    proceso->estado = READY;
    loggearCambioDeEstado(proceso->pid, anterior, proceso->estado);

    ingresarAReady(proceso); 



}

// caso bloqueo es por I/O
void bloqueoIO(int tiempo)
{

    pthread_t pcb_bloqueado;

    // Esto no bloquea el PCB, no hace nada en absoluto, tendrian que mover todo lo que esta en io_s() al hilo, porque sino no funciona, no bloquea nada.

    if (!pthread_create(&pcb_bloqueado, NULL, (void *)bloquearIO, (void *)&tiempo))
    {
        pthread_detach(pcb_bloqueado);
    }
    else
    {
        log_error(loggerError, "Error en la creacion de hilo para realizar I/O, Abort");
        abort();
    }
}

void bloquearIO(int tiempo)
{   
    sleep(tiempo); // sleep por la cantidad indicada en el motivo
}

void yield_s(t_pcb *proceso, char **parametros)
{   
    estimacionNuevaRafaga(proceso); 
    estadoProceso estadoAnterior = proceso->estado;
    proceso->estado = READY;
    loggearCambioDeEstado(proceso->pid, estadoAnterior, proceso->estado);
    ingresarAReady(proceso); 

}

void exit_s(t_pcb *proceso, char **parametros){   
    
    estadoProceso anterior = proceso->estado; 
    proceso->estado = SALIDA; 
    loggearSalidaDeProceso(proceso,parametros[0]); 
    loggearCambioDeEstado(proceso->pid, anterior, proceso->estado); 
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

void loggearBloqueoDeProcesos(t_pcb* pcb, char* motivo) {

    log_info(logger,"PID: <%d> - Bloqueado por: %s", pcb->pid, motivo); 
}

void loggearSalidaDeProceso(t_pcb* pcb, char* motivo) {

    log_info(logger,"Finaliza el proceso <%d> - Motivo: <%s>", pcb->pid, motivo); 
}