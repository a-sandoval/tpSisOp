/* KERNEL- cliente | CPU - servidor*/
#include "kernel/include/conexionCPU.h"


t_buffer* bufferContexto;
t_contexto* contextoEjecucion;
int conexionACPU;

void conexionCPU() {
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

int procesarPCB(t_pcb* procesoEnEjecucion) {

    iniciarContexto();
    bufferContexto = malloc(sizeof(t_buffer));

    logger = cambiarNombre("Kernel-CPU");
    asignarPCBAContexto(procesoEnEjecucion);
    enviar_contexto();

    // aca a su vez hay que recibir el contexto actualizado que mande la cpu, deserializarlo y cambiarlo en el PCB
    //Noc si esto iria aca porque en realidad seria como un case compartido con las cosas
    //que tiene que recibir de memoria y fs tm no?
    /*
    int operacion=recibir_operacion();

    switch(operacion){
        case CONTEXTOEJECUCION:
            recibirContextoActualizado(); //me carga el contexto actualizado en el mismo contextoEjecucion;
            actualizarPCB(procesoEnEjecucion);
            destroyContexto();
    }
*/
    //close(conexionACPU);
    
    return 0;
 
}

void iniciarContexto(){
    contextoEjecucion = malloc(sizeof(t_contexto));
    contextoEjecucion->estado = 0;
	contextoEjecucion->instrucciones = list_create();
	contextoEjecucion->instruccionesLength = list_size(contextoEjecucion->instrucciones);
	contextoEjecucion->pid = 0;
	contextoEjecucion->programCounter = 0;
	contextoEjecucion->registrosCPU = dictionary_create();
	contextoEjecucion->tablaDeArchivos = list_create();
	contextoEjecucion->tablaDeArchivosSize = list_size(contextoEjecucion->tablaDeArchivos);
	contextoEjecucion->tablaDeSegmentos = list_create();
	contextoEjecucion->tablaDeSegmentosSize = list_size(contextoEjecucion->tablaDeSegmentos);
}

// FUNCIONES PARA ENVIO DE CONTEXTO DE EJECUCION

void asignarPCBAContexto(t_pcb*  proceso){

    contextoEjecucion->estado = proceso->estado;
    contextoEjecucion->instrucciones = proceso->instrucciones;
    contextoEjecucion->pid = proceso->pid;
    contextoEjecucion->programCounter = proceso->programCounter;
    contextoEjecucion->registrosCPU = proceso->registrosCPU;
    contextoEjecucion->tablaDeArchivos = proceso->tablaDeArchivos;
    contextoEjecucion->tablaDeSegmentos = proceso->tablaDeSegmentos;
    contextoEjecucion->instruccionesLength = 0;
}

void enviar_contexto(){ 
    t_paquete* paquete = crearPaquete();
    
    paquete->codigo_operacion = CONTEXTOEJECUCION;
   
    // cargo todos los valores en el paquete
    agregarAPaquete(paquete,(void *)&contextoEjecucion->pid, sizeof(contextoEjecucion->pid));
    agregarAPaquete(paquete,(void *)&contextoEjecucion->programCounter, sizeof(contextoEjecucion->programCounter));
    agregarAPaquete(paquete,(void *)&contextoEjecucion->estado, sizeof(estadoProceso));

    agregarInstruccionesAPaquete(paquete, contextoEjecucion->instrucciones);

    agregarRegistrosAPaquete(paquete, contextoEjecucion->registrosCPU);

    //no sabemos listas de que son estas tablas entonces aun no podemos serializar o hay que serializarlo como listas y ver si dsps cambia
    //agregarAPaquete(paquete,(void *)&contextoEjecucion->tablaDeArchivosSize, sizeof(contextoEjecucion->tablaDeArchivosSize));
    //agregarAPaquete(paquete,contextoEjecucion->tablaDeArchivos, contextoEjecucion->tablaDeArchivosSize);
    //agregarAPaquete(paquete,(void *)&contextoEjecucion->tablaDeSegmentosSize, sizeof(contextoEjecucion->tablaDeSegmentosSize));
    //agregarAPaquete(paquete,contextoEjecucion->tablaDeSegmentos, contextoEjecucion->tablaDeSegmentosSize);
    

    enviarPaquete(paquete, conexionACPU);

	eliminarPaquete(paquete);
}

void agregarInstruccionesAPaquete(t_paquete* paquete, t_list* instrucciones){

    contextoEjecucion->instruccionesLength = list_size(instrucciones);
    
    agregarAPaquete(paquete, &contextoEjecucion->instruccionesLength, sizeof(uint32_t)); //primero envio la cantidad de elementos
    uint32_t i;
    for(i=0;i<contextoEjecucion->instruccionesLength;i++){
        agregarAPaquete (paquete, list_get(instrucciones, i), sizeof(char) * strlen(list_get(instrucciones, i)) + 1 );
    }
}

void agregarRegistrosAPaquete(t_paquete* paquete, t_dictionary* registrosCPU){
     
     // no terminaran en /0???
    char* AX = dictionary_get(registrosCPU,"AX"); 
    agregarAPaquete(paquete, AX, sizeof(char) * 4);
    free(AX);

    char* BX = dictionary_get(registrosCPU,"BX");
    agregarAPaquete(paquete, BX, sizeof(char) * 4 );
    free(BX);

    char* CX = dictionary_get(registrosCPU,"CX");
    agregarAPaquete(paquete, CX, sizeof(char) * 4 );
    free(CX);

    char* DX = dictionary_get(registrosCPU,"DX");
    agregarAPaquete(paquete, DX, sizeof(char) * 4 );
    free(DX);

    char* EAX = dictionary_get(registrosCPU,"EAX");
    agregarAPaquete(paquete, EAX, sizeof(char) * 8 );
    free(EAX);

    char* EBX = dictionary_get(registrosCPU,"EBX");
    agregarAPaquete(paquete, EBX, sizeof(char) * 8 );
    free(EBX);

    char* ECX = dictionary_get(registrosCPU,"ECX");
    agregarAPaquete(paquete, ECX, sizeof(char) * 8 );
    free(ECX);

    char* EDX = dictionary_get(registrosCPU,"EDX");
    agregarAPaquete(paquete, EDX, sizeof(char) * 8 );
    free(EDX);

    char* RAX = dictionary_get(registrosCPU,"RAX");
    agregarAPaquete(paquete, RAX, sizeof(char) * 16 );
    free(RAX);

    char* RBX = dictionary_get(registrosCPU,"RBX");
    agregarAPaquete(paquete, RBX, sizeof(char) * 16 );
    free(RBX);

    char* RCX = dictionary_get(registrosCPU,"RCX");
    agregarAPaquete(paquete, RCX, sizeof(char) * 16 );
    free(RCX);

    char* RDX = dictionary_get(registrosCPU,"RDX");
    agregarAPaquete(paquete, RDX, sizeof(char) * 16 );
    free(RDX);
}


//FUNCIONES PARA RECIBIR NUEVO CONTEXTO POR PARTE DE LA CPU
void recibirContextoActualizado(){
    //deserializar

	int size;
	int desplazamiento = 0;
	void * buffer;

	buffer = recibir_buffer(&size);
	while(desplazamiento < size){ //segun entiendo el while hace que se quede esperando a recbibir absoulamente todos los datos
        memcpy(&(contextoEjecucion->pid), buffer + desplazamiento, sizeof(uint32_t));
        desplazamiento+=sizeof(contextoEjecucion->pid);
        memcpy(&(contextoEjecucion->programCounter), buffer+desplazamiento, sizeof(uint32_t));
        desplazamiento+=sizeof(contextoEjecucion->programCounter);
        memcpy(&(contextoEjecucion->estado), buffer+desplazamiento, sizeof(estadoProceso));
        desplazamiento+=sizeof(estadoProceso);

        //no haria falta volver a recibir las instrucciones porque no cambian

        //SE PODRIA LLEGAR A PONER EN UNA FUNCION APARTE PERO HAY QUE MANDAR ALGUNOS PARAMETROS
        //recibirRegistros();

        char* AX;
        memcpy(&(AX), buffer + desplazamiento, sizeof(char) * 4);
        dictionary_put(contextoEjecucion->registrosCPU, "AX", AX);
        free(AX);

        char* BX;
        memcpy(&(BX), buffer + desplazamiento, sizeof(char) * 4);
        dictionary_put(contextoEjecucion->registrosCPU, "BX", BX);
        free(BX);

        char* CX;
        memcpy(&(CX), buffer + desplazamiento, sizeof(char) * 4);
        dictionary_put(contextoEjecucion->registrosCPU, "CX", CX);
        free(CX);

        char* DX;
        memcpy(&(DX), buffer + desplazamiento, sizeof(char) * 4);
        dictionary_put(contextoEjecucion->registrosCPU, "DX", DX);
        free(DX);

        char* EAX;
        memcpy(&(EAX), buffer + desplazamiento, sizeof(char) * 8);
        dictionary_put(contextoEjecucion->registrosCPU, "EAX", EAX);
        free(EAX);

        char* EBX;
        memcpy(&(EBX), buffer + desplazamiento, sizeof(char) * 8);
        dictionary_put(contextoEjecucion->registrosCPU, "EBX", EBX);
        free(EBX);

        char* ECX;
        memcpy(&(ECX), buffer + desplazamiento, sizeof(char) * 8);
        dictionary_put(contextoEjecucion->registrosCPU, "ECX", ECX);
        free(ECX);

        char* EDX;
        memcpy(&(EDX), buffer + desplazamiento, sizeof(char) * 8);
        dictionary_put(contextoEjecucion->registrosCPU, "EDX", EDX);
        free(EDX);

        char* RAX;
        memcpy(&(RAX), buffer + desplazamiento, sizeof(char) * 16);
        dictionary_put(contextoEjecucion->registrosCPU, "RAX", RAX);
        free(RAX);

        char* RBX;
        memcpy(&(RBX), buffer + desplazamiento, sizeof(char) * 16);
        dictionary_put(contextoEjecucion->registrosCPU, "RBX", RBX);
        free(RBX);

        char* RCX;
        memcpy(&(RCX), buffer + desplazamiento, sizeof(char) * 16);
        dictionary_put(contextoEjecucion->registrosCPU, "RCX", RCX);
        free(RCX);

        char* RDX;
        memcpy(&(RDX), buffer + desplazamiento, sizeof(char) * 16);
        dictionary_put(contextoEjecucion->registrosCPU, "RDX", RDX);
        free(RDX);

        //recibirTablaDeArchivos();

        //recibirTablaDeSegmentos();
		
	}

	free(buffer);

}

void* recibirTablaDeArchivos(){

}

void* recibirTablaDeSegmentos(){

}

void actualizarPCB(t_pcb* proceso){
    proceso->estado = contextoEjecucion->estado;
    proceso->instrucciones = contextoEjecucion->instrucciones;
    proceso->pid = contextoEjecucion->pid;
    proceso->programCounter = contextoEjecucion->programCounter;
    proceso->registrosCPU = contextoEjecucion->registrosCPU;
    proceso->tablaDeArchivos = contextoEjecucion->tablaDeArchivos;
    proceso->tablaDeSegmentos = contextoEjecucion->tablaDeSegmentos;

}


void destroyContexto(t_contexto *contexto) {
	list_destroy_and_destroy_elements(contexto->instrucciones, free);
	list_destroy_and_destroy_elements(contexto->tablaDeArchivos, free);
	list_destroy_and_destroy_elements(contexto->tablaDeSegmentos, free);
	dictionary_destroy_and_destroy_elements(contexto->registrosCPU, free);
}