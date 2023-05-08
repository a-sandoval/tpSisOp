#include "cpu/include/servidorKernel.h"

int ejecutarServidorCPU();
t_contexto *recibirPCB();

void escucharAlKernel() {
    char *puertoEscucha = confGet("PUERTO_ESCUCHA"); 
    socketCliente = alistarServidor(puertoEscucha);
    ejecutarServidorCPU(socketCliente);  
}

void iterator(void *value){
	
}

int ejecutarServidorCPU(){
	t_contexto* contexto;
	while (1) {
		int codOP = recibir_operacion();
		switch (codOP) {
			case -1:
				//log_info(logger, "El cliente se desconecto");
				return EXIT_FAILURE;
			case PAQUETE:
				recibirContextoActualizado();
				list_iterate(contextoEjecucion->instrucciones, iterator);
				break;
			default:
				log_warning(loggerError,"Operacion desconocida. No quieras meter la pata");
				break;
		}
	}
}

void recibirContextoActualizado(){
    //deserializar

	iniciarContexto();
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

        // Cantidad de instrucciones
	    desplazamiento+=sizeof(int);
	    memcpy(&(contextoEjecucion->instruccionesLength), buffer + desplazamiento, sizeof(uint32_t));
	    desplazamiento+=sizeof(uint32_t);

	    // Lista de instrucciones
	    log_info(logger, "Se recibieron %d instrucciones", contextoEjecucion->instruccionesLength);

	    for(uint32_t i = 0; i < contextoEjecucion->instruccionesLength; i++) {
		    memcpy(&tamanio, buffer + desplazamiento, sizeof(int));
		    desplazamiento+=sizeof(int);
		    char* valor = malloc(tamanio);
		    memcpy(valor, buffer+desplazamiento, tamanio);
		    desplazamiento+=tamanio;
		    list_add(contextoEjecucion->instrucciones, valor);
	    }

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

	free(buffer);
	
}