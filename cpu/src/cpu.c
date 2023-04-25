#include "cpu/include/cpu.h"

int main(void){

	
	logger = iniciarLogger("cpu.log", "CPU-Memoria");

	
	config = iniciarConfiguracion("cpu.config");
	
 	int conexionAMemoria = conexion("CPU", "MEMORIA");
	
	
	cambiarNombre("CPU-Kernel");
    escucharAlKernel();
	
	close(conexionAMemoria);

	terminarPrograma(); 

    return EXIT_SUCCESS;

	//recibirContextoEjecucion(t_pcb pcbParaSacarCosas); TENGO QUE HACERLA
	//ejecutarInstruccion(t_pcb pcb, t_instruccion instruccion);
}

/*
ejecutarInstruccion(t_pcb pcb, t_instruccion instruccion){
	if(instruccion está en la lista de instrucciones el pcb){
		cicloDeInstruccion(instruccion);
	} else {
		prinf("ERROR: No es posible realizar la instruccion");
	}
}
*/

void iterator(void *value){
	log_info(logger, "Valor recibido: %s\n",value);
}

