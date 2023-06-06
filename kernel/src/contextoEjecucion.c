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
    contextoEjecucion->motivoDesalojo->parametros[0] = "";
    contextoEjecucion->motivoDesalojo->parametros[1] = "";
    contextoEjecucion->motivoDesalojo->parametros[2] = "";
    contextoEjecucion->motivoDesalojo->parametrosLength = 0;
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

void agregarInstruccionesAPaquete (t_paquete* paquete, t_list* instrucciones){
    agregarAPaquete(paquete, &contextoEjecucion->instruccionesLength, sizeof(uint32_t)); //primero envio la cantidad de elementos
    for (uint32_t i = 0; i < contextoEjecucion->instruccionesLength; i++) 
        agregarAPaquete (paquete, list_get(instrucciones, i), sizeof(char) * (strlen(list_get(instrucciones, i)) + 1 ));
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
    agregarAPaquete(paquete, (void *)&motivoDesalojo->comando, sizeof(motivoDesalojo->comando));
    agregarAPaquete(paquete, (void *)&motivoDesalojo->parametrosLength, sizeof(motivoDesalojo->parametrosLength));
    for (int i = 0; i < motivoDesalojo->parametrosLength; i++)
        agregarAPaquete(paquete, (void *)&(motivoDesalojo->parametros[i]), (strlen(motivoDesalojo->parametros[i]) + 1) * sizeof(char));
}

//FUNCIONES PARA RECIBIR NUEVO CONTEXTO POR PARTE DE LA CPU
void recibirContextoActualizado(){
    //deserializar

	int size;
	int desplazamiento = 0;
    int tamanio;
	void * buffer;

	buffer = recibirBuffer(conexionACPU, &size);
        desplazamiento += sizeof(int);
        memcpy(&(contextoEjecucion->pid), buffer + desplazamiento, sizeof(uint32_t));
        desplazamiento += sizeof(contextoEjecucion->pid) + sizeof(int);
        memcpy(&(contextoEjecucion->programCounter), buffer+desplazamiento, sizeof(uint32_t));
        desplazamiento += sizeof(contextoEjecucion->programCounter) + sizeof(int);

        dictionary_clean_and_destroy_elements(contextoEjecucion->registrosCPU, free);
        
        char *temp, name[3] = "AX", longName[4] = "EAX";

        for (int i = 0; i < 3; i++) {
            ssize_t tamanioActual = sizeof(char) * (4 * pow(2, i) + 1);
            for (int j = 0; j < 4; j++) {
                temp = malloc (tamanioActual);
                memcpy (temp, buffer + desplazamiento, tamanioActual);
                dictionary_put (contextoEjecucion->registrosCPU, (i) ? longName : name, string_duplicate (temp));
                desplazamiento += tamanioActual + sizeof(int);
                free (temp);
            }
            name[0]++, longName[1]++, longName[0] = (i == 2) ? 'R' : 'E';
        }

        //recibirTablaDeArchivos();
        //recibirTablaDeSegmentos();
        //recibirMotivoDeDesalojo()

        memcpy(&(contextoEjecucion->motivoDesalojo->comando), buffer + desplazamiento, sizeof(t_comando));
        desplazamiento += sizeof(t_comando) + sizeof(int);

        memcpy(&(contextoEjecucion->motivoDesalojo->parametrosLength), buffer + desplazamiento, sizeof(uint32_t));
        desplazamiento += sizeof(contextoEjecucion->motivoDesalojo->parametrosLength);

        for (int i = 0; i < contextoEjecucion->motivoDesalojo->parametrosLength; i++) {
            memcpy(&tamanio, buffer + desplazamiento, sizeof(int));
            desplazamiento += sizeof(int);
            char *temp = malloc (tamanio);
            contextoEjecucion->motivoDesalojo->parametros[i] = temp;
            memcpy(temp, buffer + desplazamiento, tamanio);
            desplazamiento += tamanio;
        }
        desplazamiento += sizeof(int);
		memcpy(&(contextoEjecucion->rafagaCPUEjecutada), buffer + desplazamiento, sizeof(uint64_t));

	free(buffer);

}

void* recibirTablaDeArchivos(){
    return NULL;

}

void* recibirTablaDeSegmentos(){
    return NULL;

}

void destroyContexto() {
    list_destroy_and_destroy_elements(contextoEjecucion->instrucciones, free);
    dictionary_destroy_and_destroy_elements(contextoEjecucion->registrosCPU, free);
    list_destroy_and_destroy_elements(contextoEjecucion->tablaDeArchivos, free);
    list_destroy_and_destroy_elements(contextoEjecucion->tablaDeSegmentos, free);
    free(contextoEjecucion->motivoDesalojo->parametros[0]);
    free(contextoEjecucion->motivoDesalojo->parametros[1]);
    free(contextoEjecucion->motivoDesalojo->parametros[2]);
    free(contextoEjecucion->motivoDesalojo);
    free(contextoEjecucion);
}

/*void iterator(void *value){
	free(value);
}*/ 