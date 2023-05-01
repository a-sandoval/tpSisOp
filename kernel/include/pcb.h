#ifndef PCB_H_
#define PCB_H_

#include<commons/collections/list.h>
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

typedef struct  {

    int32_t pid; 
    int socketPCB; 
    t_list* instrucciones; 
    int programCounter;   
    estadoProceso estado; 
    t_reg registrosCPU;   // Puntero o no? 
    int estimadoProximaRafaga; 
    t_temporal* llegadaAReady; 
    
    //TO DO: TABLA DE SEGMENTOS Y TABLA DE ARCHIVOS 

} t_pcb; 



t_pcb* PCB;



#endif