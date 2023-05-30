#include "kernel/include/contextoEjecucion.h"

extern t_contexto* contextoEjecucion;

void iniciarContexto(){
    contextoEjecucion = malloc(sizeof(t_contexto));
	contextoEjecucion->instrucciones = NULL;
	contextoEjecucion->instruccionesLength = 0;
	contextoEjecucion->pid = 0;
	contextoEjecucion->programCounter = 0;
	contextoEjecucion->registrosCPU = NULL;
	contextoEjecucion->tablaDeArchivos = NULL;
	contextoEjecucion->tablaDeArchivosSize = 0;
	contextoEjecucion->tablaDeSegmentos = NULL;
	contextoEjecucion->tablaDeSegmentosSize = 0;
    contextoEjecucion->rafagaCPUEjecutada = 0;
    contextoEjecucion->motivoDesalojo = (t_motivoDeDesalojo *)malloc(sizeof(t_motivoDeDesalojo));
    contextoEjecucion->motivoDesalojo->parametros[0] = NULL;
    contextoEjecucion->motivoDesalojo->parametros[1] = NULL;
    contextoEjecucion->motivoDesalojo->parametros[2] = NULL;
}





//FUNCIONES PARA ENVIO DE CONTEXTO

void enviarContexto(){ 
    t_paquete* paquete = crearPaquete();
    
    paquete->codigo_operacion = CONTEXTOEJECUCION;
   
    agregarAPaquete(paquete,(void *)&contextoEjecucion->pid, sizeof(contextoEjecucion->pid));
    agregarAPaquete(paquete,(void *)&contextoEjecucion->programCounter, sizeof(contextoEjecucion->programCounter));

    agregarInstruccionesAPaquete(paquete, contextoEjecucion->instrucciones);

    agregarRegistrosAPaquete(paquete, contextoEjecucion->registrosCPU);

    //no sabemos listas de que son estas tablas entonces aun no podemos serializar o hay que serializarlo como listas y ver si dsps cambia
    //agregarAPaquete(paquete,(void *)&contextoEjecucion->tablaDeArchivosSize, sizeof(contextoEjecucion->tablaDeArchivosSize));
    //agregarAPaquete(paquete,contextoEjecucion->tablaDeArchivos, contextoEjecucion->tablaDeArchivosSize);
    //agregarAPaquete(paquete,(void *)&contextoEjecucion->tablaDeSegmentosSize, sizeof(contextoEjecucion->tablaDeSegmentosSize));
    //agregarAPaquete(paquete,contextoEjecucion->tablaDeSegmentos, contextoEjecucion->tablaDeSegmentosSize);
    
    agregarMotivoAPaquete(paquete, contextoEjecucion->motivoDesalojo);
    agregarAPaquete(paquete, (void *)&contextoEjecucion->rafagaCPUEjecutada, sizeof(contextoEjecucion->rafagaCPUEjecutada));

    enviarPaquete(paquete, conexionACPU);

	eliminarPaquete(paquete);
}

void agregarInstruccionesAPaquete(t_paquete* paquete, t_list* instrucciones){
    
    agregarAPaquete(paquete, &contextoEjecucion->instruccionesLength, sizeof(uint32_t)); //primero envio la cantidad de elementos
    uint32_t i;
    for(i=0;i<contextoEjecucion->instruccionesLength;i++){
        agregarAPaquete (paquete, list_get(instrucciones, i), sizeof(char) * (strlen(list_get(instrucciones, i)) + 1 ));
    }
}

void agregarRegistrosAPaquete(t_paquete* paquete, t_dictionary* registrosCPU) {

    char *AX = dictionary_get(registrosCPU,"AX"); 
    char* BX = dictionary_get(registrosCPU,"BX");
    char* CX = dictionary_get(registrosCPU,"CX");
    char* DX = dictionary_get(registrosCPU,"DX");
    char* EAX = dictionary_get(registrosCPU,"EAX");
    char* EBX = dictionary_get(registrosCPU,"EBX");
    char* ECX = dictionary_get(registrosCPU,"ECX");
    char* EDX = dictionary_get(registrosCPU,"EDX");
    char* RAX = dictionary_get(registrosCPU,"RAX");
    char* RBX = dictionary_get(registrosCPU,"RBX");
    char* RCX = dictionary_get(registrosCPU,"RCX");
    char* RDX = dictionary_get(registrosCPU,"RDX");
    
    agregarAPaquete(paquete, AX, sizeof(char) * (4 + 1));
    agregarAPaquete(paquete, BX, sizeof(char) * (4 + 1));
    agregarAPaquete(paquete, CX, sizeof(char) * (4 + 1));
    agregarAPaquete(paquete, DX, sizeof(char) * (4 + 1));
    agregarAPaquete(paquete, EAX, sizeof(char) * (8 + 1));
    agregarAPaquete(paquete, EBX, sizeof(char) * (8 + 1));
    agregarAPaquete(paquete, ECX, sizeof(char) * (8 + 1));
    agregarAPaquete(paquete, EDX, sizeof(char) * (8 + 1));
    agregarAPaquete(paquete, RAX, sizeof(char) * (16 + 1));
    agregarAPaquete(paquete, RBX, sizeof(char) * (16 + 1));
    agregarAPaquete(paquete, RCX, sizeof(char) * (16 + 1));
    agregarAPaquete(paquete, RDX, sizeof(char) * (16 + 1));    

}

void agregarMotivoAPaquete(t_paquete* paquete, t_motivoDeDesalojo* motivoDesalojo){

    agregarAPaquete(paquete,(void *)&motivoDesalojo->comando, sizeof(motivoDesalojo->comando));

    agregarAPaquete(paquete,(void *)&motivoDesalojo->parametrosLength, sizeof(motivoDesalojo->parametrosLength));
    agregarAPaquete(paquete,(void *)motivoDesalojo->parametros,(strlen(motivoDesalojo->parametros[0]) + strlen(motivoDesalojo->parametros[1]) + strlen(motivoDesalojo->parametros[2])) * sizeof(char)+ 1);
}

//FUNCIONES PARA RECIBIR NUEVO CONTEXTO POR PARTE DE LA CPU
void recibirContextoActualizado(){
    //deserializar

	int size;
	int desplazamiento = 0;
	void * buffer;

	buffer = recibirBuffer(&size);
        desplazamiento += sizeof(int);
        memcpy(&(contextoEjecucion->pid), buffer + desplazamiento, sizeof(uint32_t));
        desplazamiento += sizeof(contextoEjecucion->pid) + sizeof(int);
        memcpy(&(contextoEjecucion->programCounter), buffer+desplazamiento, sizeof(uint32_t));
        desplazamiento += sizeof(contextoEjecucion->programCounter) + sizeof(int);

        dictionary_clean_and_destroy_elements(contextoEjecucion->registrosCPU, free);
        char* AX = malloc(sizeof(char) * (4 + 1));
        memcpy(AX, buffer + desplazamiento, sizeof(char) * (4 + 1));
        dictionary_put(contextoEjecucion->registrosCPU, "AX", AX);
        desplazamiento += sizeof(char) * (4 + 1) + sizeof(int);

        char* BX = malloc(sizeof(char) * (4 + 1));
        memcpy(BX, buffer + desplazamiento, sizeof(char) * (4 + 1));
        dictionary_put(contextoEjecucion->registrosCPU, "BX", BX);
        desplazamiento += sizeof(char) * (4 + 1) + sizeof(int);

        char* CX = malloc(sizeof(char) * (4 + 1));
        memcpy(CX, buffer + desplazamiento, sizeof(char) * (4 + 1));
        dictionary_put(contextoEjecucion->registrosCPU, "CX", CX);
        desplazamiento += sizeof(char) * (4 + 1) + sizeof(int);

        char* DX = malloc(sizeof(char) * (4 + 1));
        memcpy(DX, buffer + desplazamiento, sizeof(char) * (4 + 1));
        dictionary_put(contextoEjecucion->registrosCPU, "DX", DX);
        desplazamiento += sizeof(char) * (4 + 1) + sizeof(int);

        char* EAX = malloc(sizeof(char) * (8 + 1));
        memcpy(EAX, buffer + desplazamiento, sizeof(char) * (8 + 1));
        dictionary_put(contextoEjecucion->registrosCPU, "EAX", EAX);
        desplazamiento += sizeof(char) * (8 + 1) + sizeof(int);

        char* EBX = malloc(sizeof(char) * (8 + 1));
        memcpy(EBX, buffer + desplazamiento, sizeof(char) * (8 + 1));
        dictionary_put(contextoEjecucion->registrosCPU, "EBX", EBX);
        desplazamiento += sizeof(char) * (8 + 1) + sizeof(int);

        char* ECX = malloc(sizeof(char) * (8 + 1));
        memcpy(ECX, buffer + desplazamiento, sizeof(char) * (8 + 1));
        dictionary_put(contextoEjecucion->registrosCPU, "ECX", ECX);
        desplazamiento += sizeof(char) * (8 + 1) + sizeof(int);

        char* EDX = malloc(sizeof(char) * (8 + 1));
        memcpy(EDX, buffer + desplazamiento, sizeof(char) * (8 + 1));
        dictionary_put(contextoEjecucion->registrosCPU, "EDX", EDX);
        desplazamiento += sizeof(char) * (8 + 1) + sizeof(int);

        char* RAX = malloc(sizeof(char) * (16 + 1));
        memcpy(RAX, buffer + desplazamiento, sizeof(char) * (16 + 1));
        dictionary_put(contextoEjecucion->registrosCPU, "RAX", RAX);
        desplazamiento += sizeof(char) * (16 + 1) + sizeof(int);

        char* RBX = malloc(sizeof(char) * (16 + 1));
        memcpy(RBX, buffer + desplazamiento, sizeof(char) * (16 + 1));
        dictionary_put(contextoEjecucion->registrosCPU, "RBX", RBX);
        desplazamiento += sizeof(char) * (16 + 1) + sizeof(int);

        char* RCX = malloc(sizeof(char) * (16 + 1));
        memcpy(RCX, buffer + desplazamiento, sizeof(char) * (16 + 1));
        dictionary_put(contextoEjecucion->registrosCPU, "RCX", RCX);
        desplazamiento += sizeof(char) * (16 + 1) + sizeof(int);

        char* RDX = malloc(sizeof(char) * (16 + 1));
        memcpy(RDX, buffer + desplazamiento, sizeof(char) * (16 + 1));
        dictionary_put(contextoEjecucion->registrosCPU, "RDX", RDX);
        desplazamiento += sizeof(char) * (16 + 1) + sizeof(int);

        //recibirTablaDeArchivos();

        //recibirTablaDeSegmentos();

        //recibirMotivoDeDesalojo

         memcpy(&(contextoEjecucion->motivoDesalojo->comando), buffer + desplazamiento, sizeof(t_comando));
        desplazamiento += sizeof(t_comando) + sizeof(int);

        memcpy(&(contextoEjecucion->motivoDesalojo->parametrosLength), buffer + desplazamiento, sizeof(uint32_t));
        desplazamiento += sizeof(contextoEjecucion->motivoDesalojo->parametrosLength) + sizeof(int);

        memcpy(&(contextoEjecucion->motivoDesalojo->parametros), buffer + desplazamiento, (strlen(contextoEjecucion->motivoDesalojo->parametros[0]) + strlen(contextoEjecucion->motivoDesalojo->parametros[1] + strlen(contextoEjecucion->motivoDesalojo->parametros[2]))) * sizeof(char) + 1);
        desplazamiento += (strlen(contextoEjecucion->motivoDesalojo->parametros[0]) + strlen(contextoEjecucion->motivoDesalojo->parametros[1] + strlen(contextoEjecucion->motivoDesalojo->parametros[2]))) * sizeof(char) + 1 + sizeof(int);


		memcpy(&(contextoEjecucion->rafagaCPUEjecutada), buffer + desplazamiento, sizeof(uint64_t));

	free(buffer);

}

void* recibirTablaDeArchivos(){
    return NULL;

}

void* recibirTablaDeSegmentos(){
    return NULL;

}

void destroyContexto(){
	list_destroy(contextoEjecucion->instrucciones);
	list_destroy(contextoEjecucion->tablaDeArchivos);
	list_destroy(contextoEjecucion->tablaDeSegmentos);
	dictionary_destroy_and_destroy_elements(contextoEjecucion->registrosCPU, free);
    free(contextoEjecucion);
}