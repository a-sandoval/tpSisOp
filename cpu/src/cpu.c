#include "cpu/include/cpu.h"

int main(void){
	logger = iniciarLogger("cpu.log", "CPU-Memoria");
	config = iniciarConfiguracion("cpu.config");
	int conexionAMemoria = conexion("MEMORIA");

	cambiarNombre("CPU-Kernel");
    escucharAlKernel();

	close(conexionAMemoria);
	terminarPrograma(); 
    return EXIT_SUCCESS;

	t_contexto *contextoActual = recibir_contexto();
	if(contextoActual != NULL){
		cicloDeInstruccion(contextoActual);
	}else{
		log_error(logger,"No se recibió bien el contexto try again o deal with it");
	}
}