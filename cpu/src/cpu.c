
#include <commons/config.h>
#include <commons/log.h>
#include "../../shared/include/configuraciones.h"
#include "../../shared/include/utilsCliente.h"
#include "../../shared/include/utilsServidor.h"

void terminar_programa(int conexion, t_log* loggerCliente,t_log* loggerServidor,t_config* config);
int conexionMemoria(t_config* config);

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
	config=iniciarConfiguracion("/home/utnso/tp-2023-1c-toTheEnd/cpu/cpu.config",logger_clienteMemoria);//Ruta del archivo para el config ../cpu.config no anda   /home/utnso/tp-2023-1c-toTheEnd/cpu/


	/*Conexion a memoria*/
	/*conexion_memoria=conexionMemoria(config);
	if(conexion_memoria == -1){
		log_error(logger_clienteMemoria,"No se pudo crear la conexion con la memoria");
		terminar_programa(conexion_memoria,logger_clienteMemoria,logger_servidorKernel,config);
		return EXIT_FAILURE; //abort o exit?
	}*/


	/*Preparacion de la cpu para servir al kernel*/
	conexion_servir_kernel=alistarServidor(logger_servidorKernel,config_get_string_value(config,"PUERTO_ESCUCHA"));

	ejecutarServidor(conexion_servir_kernel, logger_servidorKernel,clavesValidas,1);



	terminar_programa(conexion_memoria,logger_clienteMemoria,logger_servidorKernel,config);

    return EXIT_SUCCESS;
}

void terminar_programa(int conexion, t_log* loggerCliente,t_log* loggerServidor,t_config* config)
{
	log_destroy(loggerCliente);
	log_destroy(loggerServidor);
	config_destroy(config);
	liberar_conexion(conexion);
}

int conexionMemoria(t_config* config){

	/* Variables */

	int conexion;
	char* valor = config_get_string_value(config,"CLAVE_CPU_MEMORIA");
	char* ip = config_get_string_value(config,"IP_MEMORIA");
	char* puerto = config_get_string_value(config,"PUERTO_MEMORIA");

	/*Configuraciones*/
	printf("VALOR: %s, IP: %s, PUERTO:%s",valor,ip,puerto);
	putchar('\n');

	conexion = realizarConexion(ip,puerto,valor);

	/*Liberar memoria*/
	free(valor);
	free(ip);
	free(puerto);

	return conexion;

}
