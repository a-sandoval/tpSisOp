
#include <commons/config.h>
#include <commons/log.h>
#include "../include/conexionMemoria.h"
#include "../../shared/include/configuraciones.h"
#include "../../shared/include/utilsCliente.h"
#include "../../shared/include/utilsServidor.h"

int main(int argc, char *argv[]){

	int conexion_memoria;
	int conexion_servir_kernel;

	t_log* logger_clienteMemoria;
	t_log* logger_servidorKernel;

	t_config* config;

	char* clavesValidas[]={"kernel-cpu"};


	/*Inicializando Loggers*/
	logger_clienteMemoria = iniciar_logger("CPUcliente.log","CPU-Memoria");
	logger_servidorKernel= iniciar_logger("CPUservidor.log","Kernel-CPU");

	/*Inicializando los config*/
	config=iniciarConfiguracion("../cpu.config",logger_clienteMemoria);


	/*Conexion a memoria*/
	//conexion_memoria=conexionMemoria(config);
	if(conexion_memoria == -1){
		log_error(logger_clienteMemoria,"No se pudo crear la conexion con la memoria");
		abort();
	}


	/*Preparacion de la cpu para servir al kernel*/
	//conexion_servir_kernel=alistarServidor(logger_servidorKernel,config_get_string_value(config,"PUERTO_ESCUCHA"));

	//ejecutarServidor(conexion_servir_kernel, logger_servidorKernel,clavesValidas,1);



	//terminar_programa(conexion_memoria,logger_clienteMemoria,logger_servidorKernel);

    return EXIT_SUCCESS;
}

void terminar_programa(int conexion, t_log* loggerCliente,t_log* loggerServidor)
{
	log_destroy(loggerCliente);
	log_destroy(loggerServidor);
	liberar_conexion(conexion);
}
