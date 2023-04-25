#include "cpu/include/cpu.h"

int main(void){

	t_list* clavesValidas = list_create();

	/*Inicializando Loggers*/
	logger = iniciarLogger("CPUcliente.log", "CPU-Memoria");

	/*Inicializando los config*/
	config = iniciarConfiguracion("cpu.config");
	list_add(clavesValidas, confGet("CLAVE_KERNEL_CPU"));

	/*Conexion a memoria*/
	int conexion_memoria = conexionMemoria();
	if(conexion_memoria == -1){
		log_error(logger, "No se pudo crear la conexion con la memoria");
		terminarPrograma(clavesValidas);
		close(conexion_memoria);
		return EXIT_FAILURE; //abort o exit?
	}


	/*Preparacion de la cpu para servir al kernel*/
	cambiarNombre("CPU-Kernel");
	alistarServidor(confGet("PUERTO_ESCUCHA"));

	ejecutarServidor(socketCliente);
	close(conexion_memoria);

	terminarPrograma(clavesValidas);

    return EXIT_SUCCESS;

	//recibirContextoEjecucion(t_pcb pcbParaSacarCosas); TENGO QUE HACERLA
	//ejecutarInstruccion(t-pcb pcb, t_instruccion instruccion);
}

/*
ejecutarInstruccion(t-pcb pcb, t_instruccion instruccion){
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

int conexionMemoria(){

	/* Variables */

	int conexion;
	char* valor = confGet("CLAVE_CPU_MEMORIA");
	char* ip = confGet("IP_MEMORIA");
	char* puerto = confGet("PUERTO_MEMORIA");

	/*Configuraciones*/
	//log_info(logger, "VALOR: %s, IP: %s, PUERTO:%s", valor, ip, puerto);

	conexion = realizarConexion(ip,puerto,valor);
	//log_info(logger, "VALOR DE LA CONEXION: %d", conexion);


	return conexion;

}
