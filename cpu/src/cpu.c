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
		log_error(logger,"Algoritmo invalido. Debe ingresar FIFO o HRRN");
		//“PID: <PID> - Error SEG_FAULT- Segmento: <NUMERO SEGMENTO> - Offset: <OFFSET> - Tamaño: <TAMAÑO>”
		//"PID: %d - Ejecutando: %s -  %s", contextoActual->pid, "YIELD", 0
	}
}