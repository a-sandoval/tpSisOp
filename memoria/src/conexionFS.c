#include "memoria/include/conexionFS.h"


int ejecutarServidorFS(int socket){

    socketCliente = socket; 

	log_info(logger, "Entre a servidor fs");
	while (1) {
		int codOP = recibirOperacion(socketCliente);
		switch (codOP) {
			case MENSAJE:
				log_info(logger, "Se autoriza continuar :)");
				break;
			case PAQUETE:
				break;
			case -1:
				log_info(logger, "Soy servidor fs");
				log_error(logger, "El cliente se desconecto");
				return EXIT_FAILURE; 
			default:
				log_warning(logger,"Operacion desconocida. No quieras meter la pata");
				break;
		}
	}
}
