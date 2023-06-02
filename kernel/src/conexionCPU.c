/* KERNEL- cliente | CPU - servidor*/
#include "kernel/include/conexionCPU.h"


t_buffer* bufferContexto;
t_contexto* contextoEjecucion;
int conexionACPU;

void conexionCPU() {
    logger = cambiarNombre(logger, "Kernel-CPU");
    loggerError = cambiarNombre(loggerError,"Errores Kernel-CPU");


    while(1) {
        conexionACPU = conexion("CPU");

        if(conexionACPU != -1)
            return;
        else {
            log_error(loggerError, "No se pudo conectar al servidor, socket %d, esperando 5 segundos y reintentando.", conexionACPU);
            sleep(5);
        }
    }
}

int recibirOperacionDeCPU(){ //Hay que ver esto en el utils.
	int cod_op;
    
	if(recv(conexionACPU, &cod_op, sizeof(int), MSG_WAITALL) > 0)
		return cod_op;
	else
	{
		close(conexionACPU);
		return -1;
	}
}


t_contexto* procesarPCB(t_pcb* procesoEnEjecucion) {

    iniciarContexto();

    bufferContexto = malloc(sizeof(t_buffer));

    asignarPCBAContexto(procesoEnEjecucion);

    enviarContexto();

    int operacion = recibirOperacionDeCPU();


    switch(operacion){
        case CONTEXTOEJECUCION:
            recibirContextoActualizado(); 
            break;
    }

    actualizarPCB(procesoEnEjecucion);

    free(bufferContexto);
    return contextoEjecucion;
 
}

void actualizarPCB(t_pcb* proceso){
	list_destroy(proceso->instrucciones);
    proceso->instrucciones = list_duplicate(contextoEjecucion->instrucciones);
    proceso->pid = contextoEjecucion->pid;
    proceso->programCounter = contextoEjecucion->programCounter;
	dictionary_destroy_and_destroy_elements(proceso->registrosCPU, free);
    proceso->registrosCPU = registrosDelCPU(contextoEjecucion->registrosCPU);
	//list_destroy(proceso->tablaDeArchivos);
    //proceso->tablaDeArchivos = list_duplicate(contextoEjecucion->tablaDeArchivos);
	//list_destroy(proceso->tablaDeSegmentos);
    //proceso->tablaDeSegmentos = list_duplicate(contextoEjecucion->tablaDeSegmentos);

}

void asignarPCBAContexto(t_pcb* proceso){

    contextoEjecucion->instrucciones = list_duplicate(proceso->instrucciones);
    contextoEjecucion->instruccionesLength = list_size(contextoEjecucion->instrucciones);
    contextoEjecucion->pid = proceso->pid;
    contextoEjecucion->programCounter = proceso->programCounter;
    contextoEjecucion->registrosCPU = registrosDelCPU(proceso->registrosCPU);
    //contextoEjecucion->tablaDeArchivos = proceso->tablaDeArchivos;
    //contextoEjecucion->tablaDeArchivosSize = list_size(contextoEjecucion->tablaDeArchivos);
    //contextoEjecucion->tablaDeSegmentos = proceso->tablaDeSegmentos;
    //contextoEjecucion->tablaDeSegmentosSize = list_size(contextoEjecucion->tablaDeSegmentos);

}

t_dictionary *registrosDelCPU(t_dictionary *aCopiar) {
    t_dictionary *copia = dictionary_create();
    
    char* AX = malloc(sizeof(char) * (4 + 1));
    char* BX = malloc(sizeof(char) * (4 + 1));
    char* CX = malloc(sizeof(char) * (4 + 1));
    char* DX = malloc(sizeof(char) * (4 + 1));
    char* EAX = malloc(sizeof(char) * (8 + 1));
    char* EBX = malloc(sizeof(char) * (8 + 1));
    char* ECX = malloc(sizeof(char) * (8 + 1));
    char* EDX = malloc(sizeof(char) * (8 + 1));
    char* RAX = malloc(sizeof(char) * (16 + 1));
    char* RBX = malloc(sizeof(char) * (16 + 1));
    char* RCX = malloc(sizeof(char) * (16 + 1));
    char* RDX = malloc(sizeof(char) * (16 + 1));
    strncpy(AX, (char *)dictionary_get(aCopiar, "AX"), 4 + 1);
    strncpy(BX, (char *)dictionary_get(aCopiar, "BX"), 4 + 1);
    strncpy(CX, (char *)dictionary_get(aCopiar, "CX"), 4 + 1);
    strncpy(DX, (char *)dictionary_get(aCopiar, "DX"), 4 + 1);
    strncpy(EAX, (char *)dictionary_get(aCopiar, "EAX"), 8 + 1);
    strncpy(EBX, (char *)dictionary_get(aCopiar, "EBX"), 8 + 1);
    strncpy(ECX, (char *)dictionary_get(aCopiar, "ECX"), 8 + 1);
    strncpy(EDX, (char *)dictionary_get(aCopiar, "EDX"), 8 + 1);
    strncpy(RAX, (char *)dictionary_get(aCopiar, "RAX"), 16 + 1);
    strncpy(RBX, (char *)dictionary_get(aCopiar, "RBX"), 16 + 1);
    strncpy(RCX, (char *)dictionary_get(aCopiar, "RCX"), 16 + 1);
    strncpy(RDX, (char *)dictionary_get(aCopiar, "RDX"), 16 + 1);
    dictionary_put(copia, "AX", AX);
    dictionary_put(copia, "BX", BX);
    dictionary_put(copia, "CX", CX);
    dictionary_put(copia, "DX", DX);
    dictionary_put(copia, "EAX", EAX);
    dictionary_put(copia, "EBX", EBX);
    dictionary_put(copia, "ECX", ECX);
    dictionary_put(copia, "EDX", EDX);
    dictionary_put(copia, "RAX", RAX);
    dictionary_put(copia, "RBX", RBX);
    dictionary_put(copia, "RCX", RCX);
    dictionary_put(copia, "RDX", RDX);

    return copia;
}
