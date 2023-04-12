/* - Interprete entre el hardware y el software - */
#include "kernel/include/kernel.h"

int main(int argc, char *argv[]) {

    /*
	int conexion_memoria;
    int conexion_CPU;
    int conexion_FileSystem;*/

    /*Inicializando Loggers*/
	t_log* logger_clienteMemoria = iniciar_logger("KERNELcliente.log","Kernel-Memoria");
    t_log* logger_clienteCPU = iniciar_logger("KERNELcliente.log","Kernel-CPU");
    t_log* logger_clienteFileSystem = iniciar_logger("KERNELcliente.log","Kernel-FileSystem");
    
    t_log* logger_servidorConsola = iniciar_logger("KERNELservidor.log","Consola-Kernel");


	/*Inicializando los config*/
	t_config* config_clienteMemoria = iniciarConfiguracion("../kernel.config",logger_clienteMemoria);
    t_config* config_clienteCPU = iniciarConfiguracion("../kernel.config",logger_clienteCPU);
    t_config* config_clienteFileSystem = iniciarConfiguracion("../kernel.config",logger_clienteFileSystem);
    
    t_config* config_servidorConsola = iniciarConfiguracion("../kernel.config",logger_servidorConsola);

	/*Preparacion del kernel para servir a la consola*/
	int conexion_servir_consola = alistarServidor(logger_servidorConsola,config_get_string_value(config_servidorConsola,"PUERTO_ESCUCHA"));

//Todo esto debería estar en una función
	int algo = ejecutarServidor(conexion_servir_consola,logger_servidorConsola);
    if(algo == EXIT_FAILURE){
		printf("Terminando servidor");
		terminar_Programa(config_servidorConsola,logger);
		return 0; 
	}

    terminar_Programa(config_servidorConsola,logger);
	return 0;
}

void iterator(char* value) {
	log_info(logger,"%s", value);
}

void terminar_programa(t_config* config_servidorConsola, t_log* logger_ServidorConsola){
	log_destroy(logger_ServidorConsola);
	config_destroy(config_servidorConsola);
}

/* 
void terminar_programa(int conexion, t_log* logger){
	log_destroy(logger);
	liberar_conexion(conexion);
}

a desarrollar

    para crear dos sockets distintos
    int socket_cpu = crear_conexion(yada, ay), socket_memoria = crea,,, socket-_file =crear aaa

chequear que se conecte correctamente
*/