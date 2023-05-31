#include "kernel/include/syscalls.h"

t_list *recursos;
char **nombresRecursos;

void retornoContexto(t_pcb *proceso, t_contexto *contextoEjecucion){

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
        enviarMensaje("Terminado", proceso->socketPCB);
        destruirPCB(proceso);
        break;
    }
}

void wait_s(t_pcb *aEjecutar, char **parametros)
{

    char *recurso = parametros[0];

    int indexRecurso = indiceRecurso(recurso);
    //printf("Indice recurso: %d \n",indexRecurso);

    if (indexRecurso == -1)
    {
        enviarMensaje("Terminado", aEjecutar->socketPCB);
        destruirPCB(aEjecutar);
    }

    int instancRecurso = instanciasRecursos[indexRecurso];

    //printf("Instancia actual del recurso: %d \n",instancRecurso);

    instancRecurso--;

    instanciasRecursos[indexRecurso]=instancRecurso;

    //printf("Instancia actualizada: %d \n",instanciasRecursos[indexRecurso]);

    if (instancRecurso < 0)
    {
        t_list *colaBloqueadosRecurso = (t_list *)list_get(recursos, indexRecurso);

        //imprimirMatrizRecursosColaBloqueados(recursos,cantidadRecursos);

        list_add(colaBloqueadosRecurso, (void *)aEjecutar);

        //imprimirMatrizRecursosColaBloqueados(recursos,cantidadRecursos);

        aEjecutar->estado = BLOCK;
    }
}

void signal_s(t_pcb *aEjecutar, char **parametros)
{
	//printf("SIGNAL \n \n");

    char *recurso = parametros[0];

    int indexRecurso = indiceRecurso(recurso);

    //printf("Indice recurso: %d \n",indexRecurso);

    if (indexRecurso == -1)
    {
        enviarMensaje("Terminado", aEjecutar->socketPCB);
        destruirPCB(aEjecutar);
    }

    int instancRecurso = instanciasRecursos[indexRecurso];

    //printf("Instancia actual del recurso: %d \n",instancRecurso);

    instancRecurso++;

    instanciasRecursos[indexRecurso]=instancRecurso;

    //printf("Instancia actualizada: %d \n",instanciasRecursos[indexRecurso]);


    if (instancRecurso <= 0)
    {
        t_list *colaBloqueadosRecurso = (t_list *)list_get(recursos, indexRecurso);

        //printf("Antes de sacar un desbloqueado \n");
        //imprimirMatrizRecursosColaBloqueados(recursos,cantidadRecursos);

        t_pcb* pcbDesbloqueado = desencolar(colaBloqueadosRecurso);

        estadoProceso estadoAnterior = pcbDesbloqueado->estado;
        pcbDesbloqueado->estado = READY;
        
        ingresarAReady(pcbDesbloqueado); 
        //printf("Se puso en ready el pcb de pid: %d\n",pcbDesbloqueado->pid);

        //printf("Despues de sacar un desbloqueado \n");
        //imprimirMatrizRecursosColaBloqueados(recursos,cantidadRecursos);

        t_pcb *hizoSignal = aEjecutar;

        hizoSignal->estado = EXEC;

        procesarPCB(hizoSignal);//Ponerlo en ejecucion el que hizo el signal
    }

    //putchar('\n');
    //printf("SIGNAL \n \n");
}

void io_s(t_pcb *proceso, char **parametros)
{   
    estadoProceso anterior = proceso->estado;
    proceso->estado = BLOCK; 

    loggearBloqueoDeProcesos(proceso,"IO"); 
    loggearCambioDeEstado(proceso->pid, anterior, proceso->estado);
    

    int tiempo = atoi(parametros[0]);
    log_info(logger,"I/O:  'PID: <%d> - Ejecuta IO: <%d>'",proceso->pid,tiempo); 
    bloqueoIO(tiempo);

    anterior = proceso->estado;
    proceso->estado = READY;

    ingresarAReady(proceso); 

    loggearCambioDeEstado(proceso->pid, anterior, proceso->estado);


}

// caso bloqueo es por I/O
void bloqueoIO(int tiempo)
{

    pthread_t pcb_bloqueado;

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

    ingresarAReady(proceso); 

    estadoProceso estadoAnterior = proceso->estado;
    proceso->estado = READY;
    loggearCambioDeEstado(proceso->pid, estadoAnterior, proceso->estado);

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