/* - Interprete entre el hardware y el software - */
#include "kernel/include/kernel.h"

int main(){

    /*Inicializando Loggers*/
    t_log *logger_clienteMemoria = iniciar_logger("KERNELcliente.log", "Kernel-Memoria");
    t_log *logger_clienteCPU = iniciar_logger("KERNELcliente.log", "Kernel-CPU");
    t_log *logger_clienteFileSystem = iniciar_logger("KERNELcliente.log", "Kernel-FileSystem");
    t_log *logger_servidorConsola = iniciar_logger("KERNELservidor.log", "Consola-Kernel");

    /*Inicializando los config*/
    t_config *config_clienteMemoria = iniciarConfiguracion("kernel.config", logger_clienteMemoria);
    t_config *config_clienteCPU = iniciarConfiguracion("kernel.config", logger_clienteCPU);
    t_config *config_clienteFileSystem = iniciarConfiguracion("kernel.config", logger_clienteFileSystem);
    t_config *config_servidorConsola = iniciarConfiguracion("kernel.config", logger_servidorConsola);

    int conexionAMemoria = conexionMemoria(config_clienteMemoria);
    if(!(conexionAMemoria + 1)) log_error(logger_clienteMemoria, "No se pudo conectar al servidor.");
    terminar_programa(conexionAMemoria, logger_clienteMemoria, config_clienteMemoria);

    int conexionACPU = conexionCPU(config_clienteCPU);
    if(!(conexionACPU + 1)) log_error(logger_clienteCPU, "No se pudo conectar al servidor.");
    terminar_programa(conexionACPU, logger_clienteCPU, config_clienteCPU);

    int conexionAFS = conexionFileSystem(config_clienteFileSystem);
    if(!(conexionAFS + 1)) log_error(logger_clienteFileSystem, "No se pudo conectar al servidor.");
    terminar_programa(conexionAFS, logger_clienteFileSystem, config_clienteFileSystem);

    //int servidorDeConsola = 
    servirAConsola(logger_servidorConsola, config_servidorConsola);
    log_destroy(logger_servidorConsola);
    config_destroy(config_servidorConsola);

    return 0; 

}

void terminar_programa(int conexion, t_log* logger, t_config *config){
	log_destroy(logger);
    config_destroy(config); 
    close(conexion);
}