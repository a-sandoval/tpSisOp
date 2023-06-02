#ifndef SYSCALLS_H_
#define SYSCALLS_H_

#include "pcb.h"
#include "contextoEjecucion.h"
#include "semaphore.h"
#include "planificacion.h"
#include "manejoRecursos.h"
#include "algoritmosCortoPlazo.h"

extern t_list *recursos;
extern char **nombresRecursos;
extern t_contexto* contextoEjecucion;
extern sem_t hayProcesosReady;
extern int *instanciasRecursos;

void retornoContexto(t_pcb*, t_contexto*);

void signal_s(char **parametros);
void wait_s(char **parametros);
void io_s(char **parametros);
void fopen_s(char **parametros);
void fclose_s(char **parametros);
void fseek_s(char **parametros);
void fread_s(char **parametros);
void fwrite_s(char **parametros);
void createSegment_s(char **parametros);
void deleteSegment_s(char **parametros);
void yield_s();
void exit_s(char **parametros);

void bloqueoIO(int tiempo);

void bloquearIO(int tiempo);

void loggearBloqueoDeProcesos(char* motivo); 

void loggearSalidaDeProceso(char* motivo); 

#endif /* SYSCALLS_H_ */