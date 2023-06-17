#ifndef SYSCALLS_H_
#define SYSCALLS_H_

#include "semaphore.h"
#include "shared/include/contextoEjecucion.h"
#include "kernel/include/conexiones/conexionMemoria.h"
#include "kernel/include/peticiones/pcb.h"
#include "kernel/include/peticiones/manejoRecursos.h"
#include "kernel/include/peticiones/manejoArchivos.h"
#include "kernel/include/planificacion/planificacion.h"
#include "kernel/include/planificacion/algoritmosCortoPlazo.h"

extern t_list *recursos;
extern char **nombresRecursos;
extern t_contexto* contextoEjecucion;
extern sem_t hayProcesosReady;
extern int *instanciasRecursos;
extern int conexionAMemoria; 
extern int conexionAFS; 

typedef struct {

    uint32_t id; 
    uint32_t direccionBase; 
    uint32_t tamanio; 

}t_segmento; 

void retornoContexto(t_pcb*, t_contexto*);
void volverACPU(t_pcb*); 

void signal_s(t_pcb*, char **parametros);
void wait_s(t_pcb*,char **parametros);
void io_s(t_pcb*,char **parametros);
void fopen_s(t_pcb*,char **parametros);
void fclose_s(t_pcb*,char **parametros);
void fseek_s(t_pcb*,char **parametros);
void fread_s(t_pcb*,char **parametros);
void fwrite_s(t_pcb*,char **parametros);
void createSegment_s(t_pcb*,char **parametros);
void deleteSegment_s(t_pcb*,char **parametros);
void yield_s(t_pcb*);
void exit_s(t_pcb*,char **parametros);

void liberarRecursosAsignados(t_pcb* proceso);

void bloquearIO(int *tiempo);

void recibirTablaActualizada();

void loggearBloqueoDeProcesos(t_pcb*,char* motivo); 

void loggearSalidaDeProceso(t_pcb*,char* motivo); 

#endif /* SYSCALLS_H_ */