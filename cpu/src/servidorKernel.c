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
				contexto = recibirPCB();
				list_iterate(contexto->instrucciones, iterator);
				break;
			default:
				log_warning(loggerError,"Operacion desconocida. No quieras meter la pata");
				break;
		}
	}
}

t_contexto *recibirPCB(){
	int size;
	int desplazamiento = 0;
	void * buffer;
	t_contexto *contexto;
	int tamanio;

	buffer = recibir_buffer(&size);
	while(desplazamiento < size){
		memcpy(&tamanio, buffer + desplazamiento, sizeof(int));
		desplazamiento+=sizeof(int);
		char* valor = malloc(tamanio);
		memcpy(valor, buffer+desplazamiento, tamanio);
		desplazamiento+=tamanio;
		//list_add(valores, valor);
	}
	free(buffer);
	return contexto;
}