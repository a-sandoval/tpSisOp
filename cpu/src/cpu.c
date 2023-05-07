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
    

	t_contexto *contextoActual = recibir_contexto();
	if(contextoActual != NULL){
		cicloDeInstruccion(contextoActual);
	}else{
		log_error(loggerError,"No se recibió bien el contexto try again o deal with it");
	}

	return EXIT_SUCCESS;
}	