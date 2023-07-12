#include "memoria/include/conexionFS.h"


int ejecutarServidorFS(int *socketCliente){

	tiempo = config_get_int_value(config,"RETARDO_MEMORIA"); 

	log_info(logger, "Conectado el File System");
	
	while (1) {
		int peticion = recibirOperacion(*socketCliente);
		log_info(logger,"Se recibio peticion %d de File System", peticion); 
		switch (peticion) {	
			case READ:
				log_debug (logger, "Solicitud de lectura");
				recibirPeticionDeLecturaFS(*socketCliente); 
				break;
			case WRITE:
                log_debug (logger, "Solicitud de escritura");
				recibirPeticionDeEscrituraFS(*socketCliente); 
				break;
			case -1:
				log_warning (logger, "El File System se desconecto");
				return 0;
				break; 
			default:
				log_warning (logger,"Operacion desconocida. No quieras meter la pata");
				break;
		}
	}
}

//FREAD

void recibirPeticionDeLecturaFS(int socketFS) {

	int size, desplazamiento=0, pid, tamanio;
	int32_t direccionFisica;

	void* buffer = recibirBuffer(socketFS, &size);
	desplazamiento += sizeof(uint32_t);
	memcpy(&(pid), buffer + desplazamiento, sizeof(int));
	desplazamiento += sizeof(uint32_t) + sizeof (int);
	memcpy(&(direccionFisica), buffer + desplazamiento, sizeof(int32_t));
	desplazamiento += sizeof(uint32_t) + sizeof(int); 
	memcpy(&(tamanio),buffer+desplazamiento,sizeof(int)); 

	tamanio--; 

	leer_fs(direccionFisica); 

	log_info(logger, "PID: %d - Acción: %s - Dirección física: %d - Tamaño: %d - Origen: %s", pid, "READ", direccionFisica, tamanio, "FS");

	free (buffer);
}


char* leer_fs(int32_t direccionFisica) {
	usleep(tiempo *1000); 
	char* valorDirFisica = espacioDeUsuario + direccionFisica; 
	return valorDirFisica; 
}

/* VERSION POSTA DE LEER Y ESCRIBIR TANTO PARA CPU COMO FS 

char* leer(int32_t direccionFisica,int tamanio) {

	usleep(tiempo*1000); 

	char* punteroDireccionFisica = espacioDeUsuario + direccionFisica; 

	char* valorLeido = malloc(sizeof(char)*tamanio); 
	
	memcpy(valorLeido, punteroDireccionFisica, tamanio);

	return valorLeido; 

}

void escribir_fs(char* valor, int32_t direccionFisica, int tamanio){
	
	usleep(tiempo*1000); 

	char* punteroADirFisica = espacioDeUsuario + direccionFisica; 

	memcpy(punteroADirFisica, valor, tamanio);
}


*/
//FWRITE

void recibirPeticionDeEscrituraFS(int socketFS){

	int size, desplazamiento=0, tamanio, pid;
	int32_t direccionFisica;

	void* buffer = recibirBuffer(socketFS, &size);
	desplazamiento += sizeof(uint32_t);
	memcpy(&(pid), buffer + desplazamiento, sizeof(int));
	desplazamiento+=sizeof(uint32_t) + sizeof (int); 
	memcpy(&(direccionFisica), buffer + desplazamiento, sizeof(int32_t));
	desplazamiento+=sizeof(uint32_t); 
	memcpy (&(tamanio), buffer + desplazamiento, sizeof tamanio);
	desplazamiento += sizeof tamanio;
	char* valorAEscribir = malloc (sizeof (char) * tamanio);
	memcpy(valorAEscribir,buffer+desplazamiento,sizeof(char)*tamanio);
	
	tamanio--; 

	escribir_fs(valorAEscribir, direccionFisica); 

	log_info(logger, "PID: %d - Acción: %s - Dirección física: %d - Tamaño: %d - Origen: %s", pid, "WRITE", direccionFisica, tamanio, "FS");

	free (buffer);
}

void escribir_fs(char* valor, int32_t direccionFisica) {

	usleep(tiempo *1000); 
	char* punteroADirFisica = espacioDeUsuario + direccionFisica; 
	strcpy (punteroADirFisica, valor); 

}

