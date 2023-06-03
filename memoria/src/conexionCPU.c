#include "memoria/include/conexionCPU.h"


int ejecutarServidorCPU(int * socketCliente){

	log_info(logger, "Conectado el CPU.");

	while (1) {
		int peticion = recibirOperacion(*socketCliente);
		log_info(logger,"Se recibio peticion %d del CPU", peticion); 

		switch (peticion) {
			case READ:
				log_info(logger, "Solicitud de lectura");
				break;
			case WRITE:
                log_info(logger, "Solicitud de escritura");
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
