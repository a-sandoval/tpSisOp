#include "memoria/include/conexionCPU.h"

int tiempo;
char* valorDirFisica; 

int ejecutarServidorCPU(int * socketCliente){

	tiempo = config_get_int_value(config,"RETARDO_MEMORIA");

	while (1) {
		int peticion = recibirOperacion(*socketCliente);
		log_info(logger,"Se recibio peticion %d del CPU", peticion); 

		switch (peticion) {
			case READ:
				recibirPeticionDeLectura(*socketCliente);
				enviarValorObtenido(*socketCliente); 
				log_info(logger, "Solicitud de lectura");
				break;
			case WRITE:
				recibirPeticionDeEscritura(*socketCliente);
                enviarMensaje("OK",*socketCliente); 
				break;
			case -1:
				log_error(logger, "El CPU se desconecto");
				return EXIT_FAILURE;
				break; 
			default:
				log_warning(logger,"Operacion desconocida del CPU.");
				break;
		}
	}
} 

char* leer(int32_t direccionFisica){
	
	usleep(tiempo *1000); 

	valorDirFisica = espacioDeUsuario + direccionFisica; 

	return valorDirFisica; 
	
}

void recibirPeticionDeLectura(int socketCPU) {

	int size, desplazamiento=0, pid, tamanio;
	int32_t direccionFisica;

	void* buffer = recibirBuffer(socketCPU, &size);
	desplazamiento += sizeof(uint32_t);
	memcpy(&(pid), buffer + desplazamiento, sizeof(int));
	desplazamiento += sizeof(uint32_t) + sizeof (int);
	memcpy(&(direccionFisica), buffer + desplazamiento, sizeof(int32_t));
	desplazamiento += sizeof(uint32_t) + sizeof(int); 
	memcpy(&(tamanio),buffer+desplazamiento,sizeof(int)); 

	tamanio--; 

	leer(direccionFisica); 

	log_info(logger, "PID: %d - Acción: %s - Dirección física: %d - Tamaño: %d - Origen: %s", pid, "READ", direccionFisica, tamanio, "CPU");


	free (buffer);
}

void recibirPeticionDeEscritura(int socketCPU) {
	
	int size, desplazamiento=0, tamanio, pid;
	int32_t direccionFisica;

	void* buffer = recibirBuffer(socketCPU, &size);
	desplazamiento += sizeof(uint32_t);
	memcpy(&(pid), buffer + desplazamiento, sizeof(int));
	desplazamiento+=sizeof(uint32_t) + sizeof (int); 
	memcpy(&(direccionFisica), buffer + desplazamiento, sizeof(int32_t));
	desplazamiento+=sizeof(uint32_t); 
	memcpy (&(tamanio), buffer + desplazamiento, sizeof tamanio);
	desplazamiento += sizeof tamanio;
	char* valorAEscribir = malloc (sizeof (char) * tamanio);
	memcpy(valorAEscribir,buffer+desplazamiento,sizeof(char)*tamanio);
	

	escribir(valorAEscribir,direccionFisica);  


	log_info(logger, "PID: %d - Acción: %s - Dirección física: %d - Tamaño: %d - Origen: %s", pid, "WRITE", direccionFisica, tamanio, "CPU");
	free (valorAEscribir), free (buffer);
}

void enviarValorObtenido(int socketCPU){
	enviarMensaje(valorDirFisica, socketCPU);
}

void escribir(char* valor, int32_t direccionFisica){
	
	usleep(tiempo *1000); 
	char* punteroADirFisica = espacioDeUsuario + direccionFisica; 
	strcpy (punteroADirFisica, valor);
}