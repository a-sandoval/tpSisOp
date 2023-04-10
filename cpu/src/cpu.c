#include <stdlib.h>
#include <stdio.h>
#include <commons/collections/list.h>
#include <unistd.h>
#include <commons/config.h>
#include <readline/readline.h>
#include "../include/conexionMemoria.h"
#include "../include/cpu.h"
#include "../include/servidorKernel.h"

int main(int argc, char *argv[]) {

	int conexion_memoria;
	int conexion_servir_kernel;

	t_log* logger;

	/*Inicializando Loggers*/
	//logger = iniciar_logger();


	/*Conexion a memoria*/
	//conexion_memoria=conexionMemoria();


	/*Preparacion de la cpu para servir al kernel*/
	//conexion_servir_kernel=alistarServidor();

	//ejecutarServidor(conexion_servir_kernel);



	//terminar_programa(conexion_memoria,logger);

    return 0;
}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger = log_create("cpu.log", "Log",1,LOG_LEVEL_INFO);

	if(nuevo_logger == NULL){
		printf("No se pudo crear el logger");
		exit(1);
	}

	return nuevo_logger;
}


void terminar_programa(int conexion, t_log* logger)
{
	log_destroy(logger);
	liberar_conexion(conexion);
}
