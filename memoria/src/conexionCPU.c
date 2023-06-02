#include "memoria/include/conexionCPU.h"


int ejecutarServidorCPU(int * socketCliente){

	log_info(logger, "Entre a servidor cpu");

	while (1) {
		int peticion = recibirOperacion(*socketCliente);
		log_info(logger,"Hice peticion %d", peticion); 

		switch (peticion) {
			case READ:
				log_info(logger, "Solicitud de lectura");
				break;
			case WRITE:
                log_info(logger, "Solicitud de escritura");
				break;
			case -1:
				log_error(logger, "El cliente se desconecto");
				return EXIT_FAILURE;
				break; 
			default:
				log_warning(logger,"Operacion desconocida. No quieras meter la pata");
				break;
		}
	}
}
