#include "test/include/test.h"

int socketCliente;
t_log *logger;
t_config *config;

int main(){
    logger = iniciarLogger("test.log", "Memoria");

	//inicio servidor y queda a la espera de clientes
	socketCliente = alistarServidor("9001");

	//int rdoEjecucion = 
	ejecutarServidorTest();

	log_destroy(logger);
	return 0;
}

void iterator(void* value) {
	log_info(logger,"%s", (char *) value);
}

void ejecutarServidorTest(){
	
	t_contexto *contexto = newContexto();
	
	for(int i = 0; i < 1000; i++) {
		int cod_op = recibir_operacion(socketCliente); 
		switch (cod_op) {
			case CONTEXTOEJECUCION:
			//case PAQUETE:
				contexto = recibirContexto();
				list_iterate(contexto->instrucciones, (void*) iterator); 
				//list_destroy_and_destroy_elements(lista, (void*)element_destroyer);
				list_destroy(contexto->instrucciones);
				break;
			case MENSAJE:
				char *mensaje = recibirMensaje();
				log_info(logger, "%s", mensaje);
				//enviarMensaje("Terminado", socketCliente);
				break;
			case -1:
				log_info(logger, "Se desconecto el cliente");
				return;
			default:
				break;
		}
	}
	
	destroyContexto(contexto);
}

t_contexto *recibirContexto() {
	int size;
	int desplazamiento = 0;
	void * buffer;
	t_contexto *valores = newContexto();
	int tamanio;
	recv(socketCliente, &size, sizeof(int), MSG_WAITALL);
	buffer = malloc(size);
	recv(socketCliente, buffer, size, MSG_WAITALL);

	// PID, Program Counter y Estado
	memcpy(&tamanio, buffer + desplazamiento, sizeof(int));
	desplazamiento+=sizeof(int);
	memcpy(&(valores->pid), buffer + desplazamiento, tamanio);
	desplazamiento+=tamanio;
	memcpy(&tamanio, buffer + desplazamiento, sizeof(int));
	desplazamiento+=sizeof(int);
	memcpy(&(valores->programCounter), buffer + desplazamiento, tamanio);
	desplazamiento+=tamanio;
	memcpy(&tamanio, buffer + desplazamiento, sizeof(int));
	desplazamiento+=sizeof(int);
	memcpy(&(valores->estado), buffer + desplazamiento, tamanio);
	desplazamiento+=tamanio;

	log_info(logger, "PID = %d; PC = %d; State = %d", valores->pid, valores->programCounter, valores->estado);

	// Cantidad de instrucciones
	desplazamiento+=sizeof(int);
	memcpy(&(valores->instruccionesLength), buffer + desplazamiento, sizeof(uint32_t));
	desplazamiento+=sizeof(uint32_t);

	// Lista de instrucciones
	log_info(logger, "Se recibieron %d instrucciones", valores->instruccionesLength);
	for(uint32_t i = 0; i < valores->instruccionesLength; i++) {
		memcpy(&tamanio, buffer + desplazamiento, sizeof(int));
		desplazamiento+=sizeof(int);
		char* valor = malloc(tamanio);
		memcpy(valor, buffer+desplazamiento, tamanio);
		desplazamiento+=tamanio;
		list_add(valores->instrucciones, valor);
	}
	free(buffer);
	return valores;
}

t_contexto *newContexto() {
	t_contexto *nuevo = malloc(sizeof(t_contexto));
	nuevo->estado = 0;
	nuevo->instrucciones = list_create();
	nuevo->instruccionesLength = list_size(nuevo->instrucciones);
	nuevo->pid = 0;
	nuevo->programCounter = 0;
	nuevo->registrosCPU = dictionary_create();
	nuevo->registrosSize = dictionary_size(nuevo->registrosCPU);
	nuevo->tablaDeArchivos = list_create();
	nuevo->tablaDeArchivosSize = list_size(nuevo->tablaDeArchivos);
	nuevo->tablaDeSegmentos = list_create();
	nuevo->tablaDeSegmentosSize = list_size(nuevo->tablaDeSegmentos);

	return nuevo;
}

void destroyContexto(t_contexto *contexto) {
	list_destroy_and_destroy_elements(contexto->instrucciones, free);
	list_destroy_and_destroy_elements(contexto->tablaDeArchivos, free);
	list_destroy_and_destroy_elements(contexto->tablaDeSegmentos, free);
	dictionary_destroy_and_destroy_elements(contexto->registrosCPU, free);
}