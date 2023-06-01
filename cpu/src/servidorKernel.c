#include "cpu/include/servidorKernel.h"

int ejecutarServidorCPU();
t_contexto *recibirPCB();

void escucharAlKernel() {
    char *puertoEscucha = confGet("PUERTO_ESCUCHA"); 
    socketCliente = alistarServidor(puertoEscucha);
	log_info(logger, "Kernel conectada!");
    ejecutarServidorCPU();  
    //close(socketCliente);
}

bool noEsBloqueante(t_comando instruccionActual) {
	t_comando instruccionesBloqueantes[13] = {
		IO, WAIT, SIGNAL, YIELD, EXIT, 
		F_OPEN, F_CLOSE, F_SEEK, F_READ, F_WRITE, F_TRUNCATE, 
		CREATE_SEGMENT, DELETE_SEGMENT 
	};

	for (int i = 0; i < 13; i++) 
		if (instruccionActual == instruccionesBloqueantes[i]) return false;

	return true;
}

int ejecutarServidorCPU(){
	while (1) {
        instruccionActual = -1;
		int codOP = recibirOperacion();
		switch (codOP) {
			case -1:
				log_info(logger, "La Kernel se desconecto.");
				return EXIT_FAILURE;
			case CONTEXTOEJECUCION:
				recibirContextoActualizado();
                while(contextoEjecucion->programCounter != (int) contextoEjecucion->instruccionesLength 
					  && (noEsBloqueante(instruccionActual))) {
                    cicloDeInstruccion();
                }
                destroyContexto();
				break;
			default:
				log_warning(loggerError,"Operacion desconocida. No quieras meter la pata");
				break;
		}
	}
}

