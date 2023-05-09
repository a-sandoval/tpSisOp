#ifndef PCB_H_
#define PCB_H_

#include<commons/collections/list.h>
#include<commons/collections/dictionary.h>
#include<commons/temporal.h>

typedef enum estadoProceso{
    NEW, 
    READY,
    EXEC,
    BLOCK,
    SALIDA,
} estadoProceso; 

typedef struct {
    char AX[4], BX[4], CX[4], DX[4];
    char EAX[8], EBX[8], ECX[8], EDX[8];
    char RAX[16], RBX[16], RCX[16], RDX[16];
}t_reg;

typedef struct {

    uint32_t pid; 
    int socketPCB; 
    t_list* instrucciones; 
    uint32_t programCounter;   
    estadoProceso estado; 
    t_dictionary* registrosCPU;
    int estimadoProximaRafaga; 
    t_temporal* llegadaAReady; 
    t_list* tablaDeSegmentos; 
    t_list* tablaDeArchivos; 

} t_pcb; 


#endif