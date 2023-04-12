/* - Interprete entre el hardware y el software - */
#include "kernel/include/kernel.h"

int main(int argc, char *argv[]) {

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

    int conexionMemoria(t_config* config_clienteMemoria);
    int conexionCPU(t_config* config_clienteCPU);
    int conexionFileSystem(t_config* config_clienteFileSystem);
	int servirAConsola(t_log** logger_servidorConsola,t_config* config_servidorConsola);
}

void terminar_programa(t_config* config_servidorConsola, t_log* logger_ServidorConsola){
	log_destroy(logger_ServidorConsola);
	config_destroy(config_servidorConsola);
}
/* 
void terminar_programa(int conexion, t_log* logger){
	log_destroy(logger);
	liberar_conexion(conexion);
}*/