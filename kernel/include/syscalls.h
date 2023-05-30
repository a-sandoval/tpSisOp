#ifndef SYSCALLS_H_
#define SYSCALLS_H_

#include "pcb.h"
#include "contextoEjecucion.h"
#include "semaphore.h"
#include "planificacion.h"

void retornoContexto(t_pcb *proceso, t_contexto *contextoEjecucion);

void signal_s(t_pcb *aEjecutar, char **parametros);
void wait_s(t_pcb *aEjecutar, char **parametros);
void io_s(t_pcb *aEjecutar, char **parametros);
void fopen_s(t_pcb *aEjecutar, char **parametros);
void fclose_s(t_pcb *aEjecutar, char **parametros);
void fseek_s(t_pcb *aEjecutar, char **parametros);
void fread_s(t_pcb *aEjecutar, char **parametros);
void fwrite_s(t_pcb *aEjecutar, char **parametros);
void createSegment_s(t_pcb *aEjecutar, char **parametros);
void deleteSegment_s(t_pcb *aEjecutar, char **parametros);
void yield_s(t_pcb *aEjecutar, char **parametros);
void exit_s(t_pcb *aEjecutar, char **parametros);

extern t_list *instanciasRecursos;
extern t_list *recursos;
extern char **nombresRecursos;
extern t_contexto* contextoEjecucion;
extern sem_t hayProcesosReady;

#endif /* SYSCALLS_H_ */