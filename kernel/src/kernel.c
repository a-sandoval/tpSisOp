/* - Interprete entre el hardware y el software - */
#include "kernel/include/kernel.h"

// Parametros del main que me choree de Fede: int argc, char *argv[]
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

    int conexionMemoria(t_config* config_clienteMemoria);
    int conexionCPU(t_config* config_clienteCPU);
    int conexionFileSystem(t_config* config_clienteFileSystem);
	int servirAConsola(t_log* logger_servidorConsola,t_config* config_servidorConsola);
}

/* 
VER SI COMPILA TODO
VER SI PUEDO REUTILIZAR EL TERMINAR_PROGRAMA
*/