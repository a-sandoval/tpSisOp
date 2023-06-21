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

	int size, desplazamiento=0;
	int32_t direccionFisica;

	void* buffer = recibirBuffer(socketCPU, &size);
	desplazamiento += sizeof(uint32_t);
	memcpy(&(direccionFisica), buffer + desplazamiento, sizeof(int32_t));

	leer(direccionFisica); 

	
}

void recibirPeticionDeEscritura(int socketCPU) {
	
	int size, desplazamiento=0;
	int32_t direccionFisica;
	char* valorAEscribir; 

	void* buffer = recibirBuffer(socketCPU, &size);
	desplazamiento += sizeof(uint32_t);
	memcpy(&(direccionFisica), buffer + desplazamiento, sizeof(int32_t));

	desplazamiento+=sizeof(uint32_t) + sizeof(char)*16; 

	memcpy(&valorAEscribir,buffer+desplazamiento,sizeof(char)*16);

	escribir(valorAEscribir,direccionFisica);  


	//log_info(logger, “PID: %d - Acción: %d - Dirección física: %d - Tamaño: %d - Origen: %d”, );
	//“PID: <PID> - Acción: <LEER / ESCRIBIR> - Dirección física: <DIRECCIÓN_FÍSICA> - Tamaño: <TAMAÑO> - Origen: <CPU / FS>”
}

void enviarValorObtenido(int socketCPU){
	
	t_paquete* paquete = crearPaquete(); 
	paquete->codigo_operacion=PAQUETE; 

	int32_t valorLength = strlen(valorDirFisica); 

	agregarAPaquete(paquete,(void*)&valorLength, sizeof(int32_t));
	agregarAPaquete(paquete,(void*)valorDirFisica, sizeof(char)*valorLength); 

	enviarPaquete(paquete,socketCPU); 
}

void escribir(char* valor, int32_t direccionFisica){
	
	usleep(tiempo *1000); 
	char* punteroADirFisica = espacioDeUsuario + direccionFisica; 
	strcpy(punteroADirFisica, valor);
}