#include "kernel/include/syscalls.h"

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
    case YIELD:
        yield_s(proceso, contextoEjecucion->motivoDesalojo->parametros);
        break;
    case EXIT:
        exit_s(proceso, contextoEjecucion->motivoDesalojo->parametros);
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

void signal_s(t_pcb *proceso, char **parametros)
{
    char *recurso = parametros[0];
    int indexRecurso = indiceRecurso(recurso);

    if (indexRecurso == -1)
    {
        enviarMensaje("Terminado", proceso->socketPCB);
        destruirPCB(proceso);
    }

    int instancRecurso = (int *)list_get(instanciasRecursos, indexRecurso);
    instancRecurso++;
    list_replace(instanciasRecursos, indexRecurso, (void *)&instancRecurso);

    if (instancRecurso >= 0)
    {
        t_list *colaBloqueadosRecurso = (t_list *)list_get(recursos, indexRecurso);

        list_add(colaBloqueadosRecurso, (void *)proceso);

        t_pcb *hizoSignal = (t_pcb *)list_remove(colaBloqueadosRecurso, indexRecurso);

        hizoSignal->estado = EXEC;

        procesarPCB(hizoSignal);
    }
}

void io_s(t_pcb *proceso, char **parametros)
{

    int tiempo = atoi(parametros[0]);
    bloqueoIO(proceso, tiempo);
    estadoProceso estadoAnterior = proceso->estado;
    proceso->estado = READY;
    loggearCambioDeEstado(proceso->pid, estadoAnterior, proceso->estado);
    encolar(pcbsREADY, proceso);
    sem_post(&hayProcesosReady);
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

void yield_s(t_pcb *proceso, char **parametros)
{
}

void exit_s(t_pcb *proceso, char **parametros)
{
}

*/