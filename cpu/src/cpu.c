#include <stdlib.h>
#include <stdio.h>
#include <commons/collections/list.h>
#include <unistd.h>
#include <commons/config.h>
#include <readline/readline.h>
#include "../include/conexionMemoria.h"
#include "../include/cpu.h"
#include "../include/servidorKernel.h"
#include "../../shared/include/configuraciones.h"
#include "../../shared/include/utilsServidor.h"
#include "../../shared/include/utilsCliente.h"

int main(int argc, char *argv[]) {

	int conexion_memoria;
	int conexion_servir_kernel;

	t_log* logger_clienteMemoria;
	t_log* logger_servidorKernel;

	t_config* config;

	/*Inicializando Loggers*/
	logger_clienteMemoria = iniciar_logger("CPUcliente.log","CPU-Memoria");
	logger_servidorKernel= iniciar_logger("CPUservidor.log","Kernel-CPU");

	/*Inicializando los config*/
	config=iniciarConfiguracion("../cpu.config",logger_clienteMemoria);

	/*Conexion a memoria*/
	//conexion_memoria=conexionMemoria();


	/*Preparacion de la cpu para servir al kernel*/
	conexion_servir_kernel=alistarServidor(logger_servidorKernel,config_get_string_value(config,"PUERTO_ESCUCHA"));

	//ejecutarServidor(conexion_servir_kernel,logger_servidorKernel);



	//terminar_programa(conexion_memoria,logger);

    return 0;
}

void terminar_programa(int conexion, t_log* logger)
{
	log_destroy(logger);
	liberar_conexion(conexion);
}
