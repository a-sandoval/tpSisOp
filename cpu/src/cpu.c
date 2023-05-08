#include "cpu/include/cpu.h"



int main(void){
	logger = iniciarLogger("cpu.log", "CPU-Memoria");
	loggerError = iniciarLogger("errores.log", "Errores CPU");
	
	config = iniciarConfiguracion("cpu.config");
	int conexionAMemoria = conexion("MEMORIA");

	cambiarNombre("CPU-Kernel");
    escucharAlKernel();

	close(conexionAMemoria);
	terminarPrograma(); 
    

	recibirContextoActualizado();
	if(contextoEjecucion != NULL){
		cicloDeInstruccion();
	}else{
		log_error(loggerError,"No se recibió bien el contexto try again o deal with it");
	}

	return EXIT_SUCCESS;
}	