#include "memoria/include/conexionFS.h"


int ejecutarServidorFS(int *socketCliente){

	log_info(logger, "Conectado el File System");
	
	while (1) {
		int peticion = recibirOperacion(*socketCliente);
		log_info(logger,"Se recibio peticion %d de File System", peticion); 
		switch (peticion) {	
			case READ:
				log_info (logger, "Solicitud de lectura");
				break;
			case WRITE:
                log_info (logger, "Solicitud de escritura");
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
