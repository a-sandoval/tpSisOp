#include "kernel/include/syscalls.h"

void wait_s(t_pcb *aEjecutar, char **parametros)
{

    char *recurso = parametros[0];
    int indexRecurso = indiceRecurso(recurso);

    if (indexRecurso == -1)
    {
        enviarMensaje("Terminado", aEjecutar->socketPCB);
        destruirPCB(aEjecutar);
    }

    int instancRecurso = (int *)list_get(instanciasRecursos, indexRecurso);
    instancRecurso--;
    list_replace(instanciasRecursos, indexRecurso, (void *)&instancRecurso);

    if (instancRecurso < 0)
    {
        t_list *colaBloqueadosRecurso = (t_list *)list_get(recursos, indexRecurso);

        list_add(colaBloqueadosRecurso, (void *)aEjecutar);

        aEjecutar->estado = BLOCK;
    }
}

void signal_s(t_pcb *aEjecutar, char **parametros)
{
    char *recurso = parametros[0];
    int indexRecurso = indiceRecurso(recurso);

    if (indexRecurso == -1)
    {
        enviarMensaje("Terminado", aEjecutar->socketPCB);
        destruirPCB(aEjecutar);
    }

    int instancRecurso = (int*)list_get(instanciasRecursos, indexRecurso);
    instancRecurso++;
    list_replace(instanciasRecursos, indexRecurso, (void *)&instancRecurso);

    if (instancRecurso >= 0)
    {
        t_list *colaBloqueadosRecurso = (t_list *)list_get(recursos, indexRecurso);

        list_add(colaBloqueadosRecurso, (void *)aEjecutar);

        t_pcb *hizoSignal = (t_pcb *)list_remove(colaBloqueadosRecurso, indexRecurso);

        hizoSignal->estado = EXEC;

        procesarPCB(hizoSignal);
    }
}

void io_s(t_pcb *aEjecutar, char **parametros){

	int tiempo = atoi(parametros[0]);
	bloqueoIO(aEjecutar, tiempo);
	estadoProceso estadoAnterior = aEjecutar->estado;
	aEjecutar->estado = READY;
	loggearCambioDeEstado(aEjecutar->pid, estadoAnterior, aEjecutar->estado);
	encolar(pcbsREADY, aEjecutar);
    sem_post(&hayProcesosReady);

}

void fopen_s(t_pcb *aEjecutar, char **parametros){

}

void fclose_s(t_pcb *aEjecutar, char **parametros){

}

void fseek_s(t_pcb *aEjecutar, char **parametros){

}

void fread_s(t_pcb *aEjecutar, char **parametros){

}

void fwrite_s(t_pcb *aEjecutar, char **parametros){

}

void createSegment_s(t_pcb *aEjecutar, char **parametros){

}

void deleteSegment_s(t_pcb *aEjecutar, char **parametros){

}

void yield_s(t_pcb *aEjecutar, char **parametros){

}

void exit_s(t_pcb *aEjecutar, char **parametros){

}

