/* - Interprete entre el hardware y el software - */
#include "kernel/include/kernel.h"

int main() {
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

    conexionMemoria(config_clienteMemoria);
    conexionCPU(config_clienteCPU);
    conexionFileSystem(config_clienteFileSystem);
	servirAConsola(logger_servidorConsola,config_servidorConsola);
}

/* 
VER SI COMPILA TODO
VER SI PUEDO REUTILIZAR EL TERMINAR_PROGRAMA
*/