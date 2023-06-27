#include "memoria/include/conexionCPU.h"

int tiempo;
char* valorDirFisica; 

int ejecutarServidorCPU(int * socketCliente){

	log_debug(logger, "Conectado el CPU.");
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

	int size, desplazamiento=0, pid;
	int32_t direccionFisica;

	void* buffer = recibirBuffer(socketCPU, &size);
	desplazamiento += sizeof(uint32_t);
	memcpy(&(pid), buffer + desplazamiento, sizeof(int));
	desplazamiento += sizeof(uint32_t) + sizeof (int);
	memcpy(&(direccionFisica), buffer + desplazamiento, sizeof(int32_t));

	leer(direccionFisica); 

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
	

	log_debug (logger, "Escribiendo \"%s\" en direccion física %d", valorAEscribir, direccionFisica);
	escribir(valorAEscribir,direccionFisica);  


	log_info(logger, "PID: %d - Acción: %s - Dirección física: %d - Tamaño: %d - Origen: %s", pid, "WRITE", direccionFisica, tamanio, "CPU");
	//“PID: <PID> - Acción: <LEER / ESCRIBIR> - Dirección física: <DIRECCIÓN_FÍSICA> - Tamaño: <TAMAÑO> - Origen: <CPU / FS>”
	free (valorAEscribir), free (buffer);
}

void enviarValorObtenido(int socketCPU){
	/*
	t_paquete* paquete = crearPaquete(); 
	paquete->codigo_operacion=PAQUETE; 

	int32_t valorLength = strlen(valorDirFisica) + 1; 

	//agregarAPaquete(paquete,&valorLength, sizeof(int32_t));
	agregarAPaquete(paquete,valorDirFisica, sizeof(char)*valorLength); 

	enviarPaquete(paquete,socketCPU); 
	*/
	enviarMensaje(valorDirFisica, socketCPU);
}

void escribir(char* valor, int32_t direccionFisica){
	
	usleep(tiempo *1000); 
	char* punteroADirFisica = espacioDeUsuario + direccionFisica; 
	strcpy (punteroADirFisica, valor);
	//log_debug(logger, "%s", (char *)(espacioDeUsuario + direccionFisica));
}