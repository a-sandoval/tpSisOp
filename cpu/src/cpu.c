#include "cpu/include/cpu.h"


int main(void){
	
	logger = iniciarLogger("cpu.log", "CPU");
	loggerError = iniciarLogger("errores.log", "Errores CPU");
	
	config = iniciarConfiguracion("cpu.config");
	
	conexionMemoria(); 

	cambiarNombre(logger,"CPU-Kernel");
    escucharAlKernel();

	log_destroy(loggerError);

	return EXIT_SUCCESS;
}	