#include "cpu/include/servidorKernel.h"

int ejecutarServidorCPU();
t_contexto *recibirPCB();

void escucharAlKernel() {
    char *puertoEscucha = confGet("PUERTO_ESCUCHA"); 
    socketCliente = alistarServidor(puertoEscucha);
    ejecutarServidorCPU();  
    //close(socketCliente);
}

int ejecutarServidorCPU(){
	while (1) {
        instruccionActual = -1;
		int codOP = recibirOperacion();
		switch (codOP) {
			case -1:
				log_info(logger, "La CPU se desconecto, esperando conexiones.");
				return EXIT_FAILURE;
			case CONTEXTOEJECUCION:
				recibirContextoActualizado();
                while(contextoEjecucion->programCounter != (int) contextoEjecucion->instruccionesLength && (instruccionActual != EXIT && instruccionActual != YIELD)) {
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

