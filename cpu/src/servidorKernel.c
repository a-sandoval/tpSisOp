#include "cpu/include/servidorKernel.h"

int ejecutarServidorCPU();
t_contexto *recibirPCB();

void escucharAlKernel() {
    char *puertoEscucha = confGet("PUERTO_ESCUCHA"); 
    socketCliente = alistarServidor(puertoEscucha);
    ejecutarServidorCPU();  
    //close(socketCliente);
}

void iterator(void *value){
	free(value);
}

int ejecutarServidorCPU(){
	while (1) {
        instruccionActual = -1;
		int codOP = recibirOperacion();
		switch (codOP) {
			case -1:
				log_info(logger, "La CPU se desconecto, esperando conexiones.");
				return EXIT_FAILURE;
			case CONTEXTOEJECUCION:
				recibirContextoActualizado();
                while(contextoEjecucion->programCounter != (int) contextoEjecucion->instruccionesLength && (instruccionActual != EXIT && instruccionActual != YIELD)) {
                    cicloDeInstruccion();
                }
                liberarContexto();
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
    int tamanio;
	void * buffer;

	buffer = recibirBuffer(&size);
	//while(desplazamiento < size){ //segun entiendo el while hace que se quede esperando a recbibir absoulamente todos los datos
    // El while no es necesario ya que (en un mundo ideal!) todos los datos que se mandan son atrapados aca, 
    // sino se esta enviando incorrectamente algo. Ademas el ciclo empezaria a agarrar cosas sin saber, como el PID.
        desplazamiento += sizeof(int);
        memcpy(&(contextoEjecucion->pid), buffer + desplazamiento, sizeof(uint32_t));
        desplazamiento += sizeof(contextoEjecucion->pid) + sizeof(int);
        memcpy(&(contextoEjecucion->programCounter), buffer+desplazamiento, sizeof(uint32_t));
        desplazamiento += sizeof(contextoEjecucion->programCounter) + sizeof(int);
        memcpy(&(contextoEjecucion->estado), buffer+desplazamiento, sizeof(estadoProceso));
        desplazamiento += sizeof(estadoProceso) + sizeof(int);


        // Cantidad de instrucciones
	    memcpy(&(contextoEjecucion->instruccionesLength), buffer + desplazamiento, sizeof(uint32_t));
	    desplazamiento+=sizeof(uint32_t);

	    // Lista de instrucciones
        log_info(logger, "PID = %d | Estado = %d | Program Counter = %d", contextoEjecucion->pid, contextoEjecucion->estado, contextoEjecucion->programCounter);
	    log_info(logger, "Se recibieron %d instrucciones", contextoEjecucion->instruccionesLength);

	    for(uint32_t i = 0; i < contextoEjecucion->instruccionesLength; i++) {
		    memcpy(&tamanio, buffer + desplazamiento, sizeof(int));
		    desplazamiento+=sizeof(int);
		    char* valor = malloc(tamanio);
		    memcpy(valor, buffer+desplazamiento, tamanio);
		    desplazamiento+=tamanio;
		    list_add(contextoEjecucion->instrucciones, valor);
	    }
        desplazamiento += sizeof(int);

        //SE PODRIA LLEGAR A PONER EN UNA FUNCION APARTE PERO HAY QUE MANDAR ALGUNOS PARAMETROS
        //recibirRegistros();

        char* AX = malloc(sizeof(char) * (4 + 1));
        memcpy(AX, buffer + desplazamiento, sizeof(char) * (4 + 1));
        dictionary_put(contextoEjecucion->registrosCPU, "AX", AX);
        //free(AX);
        desplazamiento += sizeof(char) * (4 + 1) + sizeof(int);
        
        char* BX = malloc(sizeof(char) * (4 + 1));
        memcpy(BX, buffer + desplazamiento, sizeof(char) * (4 + 1));
        dictionary_put(contextoEjecucion->registrosCPU, "BX", BX);
        //free(BX);
        desplazamiento += sizeof(char) * (4 + 1) + sizeof(int);

        char* CX = malloc(sizeof(char) * (4 + 1));
        memcpy(CX, buffer + desplazamiento, sizeof(char) * (4 + 1));
        dictionary_put(contextoEjecucion->registrosCPU, "CX", CX);
        //free(CX);
        desplazamiento += sizeof(char) * (4 + 1) + sizeof(int);

        char* DX = malloc(sizeof(char) * (4 + 1));
        memcpy(DX, buffer + desplazamiento, sizeof(char) * (4 + 1));
        dictionary_put(contextoEjecucion->registrosCPU, "DX", DX);
        //free(DX);
        desplazamiento += sizeof(char) * (4 + 1) + sizeof(int);

        char* EAX = malloc(sizeof(char) * (8 + 1));
        memcpy(EAX, buffer + desplazamiento, sizeof(char) * (8 + 1));
        dictionary_put(contextoEjecucion->registrosCPU, "EAX", EAX);
        //free(EAX);
        desplazamiento += sizeof(char) * (8 + 1) + sizeof(int);

        char* EBX = malloc(sizeof(char) * (8 + 1));
        memcpy(EBX, buffer + desplazamiento, sizeof(char) * (8 + 1));
        dictionary_put(contextoEjecucion->registrosCPU, "EBX", EBX);
        //free(EBX);
        desplazamiento += sizeof(char) * (8 + 1) + sizeof(int);

        char* ECX = malloc(sizeof(char) * (8 + 1));
        memcpy(ECX, buffer + desplazamiento, sizeof(char) * (8 + 1));
        dictionary_put(contextoEjecucion->registrosCPU, "ECX", ECX);
        //free(ECX);
        desplazamiento += sizeof(char) * (8 + 1) + sizeof(int);

        char* EDX = malloc(sizeof(char) * (8 + 1));
        memcpy(EDX, buffer + desplazamiento, sizeof(char) * (8 + 1));
        dictionary_put(contextoEjecucion->registrosCPU, "EDX", EDX);
        //free(EDX);
        desplazamiento += sizeof(char) * (8 + 1) + sizeof(int);

        char* RAX = malloc(sizeof(char) * (16 + 1));
        memcpy(RAX, buffer + desplazamiento, sizeof(char) * (16 + 1));
        dictionary_put(contextoEjecucion->registrosCPU, "RAX", RAX);
        //free(RAX);
        desplazamiento += sizeof(char) * (16 + 1) + sizeof(int);

        char* RBX = malloc(sizeof(char) * (16 + 1));
        memcpy(RBX, buffer + desplazamiento, sizeof(char) * (16 + 1));
        dictionary_put(contextoEjecucion->registrosCPU, "RBX", RBX);
        //free(RBX);
        desplazamiento += sizeof(char) * (16 + 1) + sizeof(int);

        char* RCX = malloc(sizeof(char) * (16 + 1));
        memcpy(RCX, buffer + desplazamiento, sizeof(char) * (16 + 1));
        dictionary_put(contextoEjecucion->registrosCPU, "RCX", RCX);
        //free(RCX);
        desplazamiento += sizeof(char) * (16 + 1) + sizeof(int);

        char* RDX = malloc(sizeof(char) * (16 + 1));
        memcpy(RDX, buffer + desplazamiento, sizeof(char) * (16 + 1));
        dictionary_put(contextoEjecucion->registrosCPU, "RDX", RDX);
        //free(RDX);
        desplazamiento += sizeof(char) * (16 + 1) + sizeof(int);

        log_info(logger, "Se recibieron los siguientes registros: ");
        log_info(logger, "AX = %s | BX = %s | CX = %s | DX = %s", AX, BX, CX, DX);
        log_info(logger, "EAX = %s | EBX = %s | ECX = %s | EDX = %s", EAX, EBX, ECX, EDX);
        log_info(logger, "RAX = %s | RBX = %s | RCX = %s | RDX = %s", RAX, RBX, RCX, RDX);
/*      
        for(int i = 0; i < 4; j++) {
            char nombre[2] = {'A', 'X'};
            nombre[0] += i;
            char *buffer2 = malloc(sizeof(char) * (4 + 1));
            memcpy(buffer2, buffer + desplazamiento, sizeof(char) * (4 + 1));
            dictionary_put(contextoEjecucion->registrosCPU, nombre, buffer2);
            desplazamiento += sizeof(char) * (4 + 1) + sizeof(int);
        }
        for(int i = 1; i < 3; i++) {
            for(int j = 0; j < 4; j++) {
                char nombre[3] = {'E', 'A', 'X'};
                nombre[1] += j;
                nombre[0] = (i == 1) ? 'E' : 'R';
                char *buffer2 = malloc(sizeof(char) * (4 * i + 1));
                memcpy(buffer2, buffer + desplazamiento, sizeof(char) * (4 * i + 1));
                dictionary_put(contextoEjecucion->registrosCPU, nombre, buffer2);
                desplazamiento += sizeof(char) * (4 + 1) + sizeof(int);
            }
        }
*/
        //recibirTablaDeArchivos();

        //recibirTablaDeSegmentos();
		
	free(buffer);

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

void liberarContexto() {
    list_destroy_and_destroy_elements(contextoEjecucion->instrucciones, iterator);
    dictionary_destroy_and_destroy_elements(contextoEjecucion->registrosCPU, iterator);
    list_destroy_and_destroy_elements(contextoEjecucion->tablaDeArchivos, iterator);
    list_destroy_and_destroy_elements(contextoEjecucion->tablaDeSegmentos, iterator);
    free(contextoEjecucion);
}