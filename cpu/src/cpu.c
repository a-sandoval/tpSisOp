#include "cpu/include/cpu.h"

int main(void){
	/*Inicializando Loggers*/
	logger = iniciarLogger("cpu.log", "CPU-Memoria");

	/*Inicializando los config*/
	config = iniciarConfiguracion("cpu.config");

	/*Conexion a memoria*/
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
ejecutarInstruccion(t-pcb pcb, t_instruccion instruccion){
	listaInstrucciones = pcb.
	if(bool list_any_satisfy(t_list* instrucciones, bool(*==instruccion)(void*));){
		cicloDeInstruccion(instruccion);
	} else {
		prinf("ERROR: No es posible realizar la instruccion");
	}
}
*/

void iterator(char *value){
	log_info(logger, "Valor recibido: %s\n",value);
}
