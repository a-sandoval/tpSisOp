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
		int offset; //ESTO ESTÁ MAL PERO LO DEJO PARA QUE NO ROMPA
		log_error(logger,"PID: %d - Error SEG_FAULT- Segmento: %s - Offset: %s - Tamanio: %s", contextoActual->pid, contextoActual->tablaDeSegmentos, offset,contextoActual->instruccionesLength);
	}
}