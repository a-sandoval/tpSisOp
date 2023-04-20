/* - Interprete entre el hardware y el software - */
#include "kernel/include/kernel.h"

int main(){

    /*Inicializando Logger*/
    logger = iniciarLogger("kernel.log", "Kernel");

    /*Inicializando el config*/
    config = iniciarConfiguracion("kernel.config", logger);

    // Generar conexion a memoria
    logger = cambiarNombre(logger, "Kernel-Memoria");
    int conexionAMemoria = conexion("MEMORIA");
    if(!(conexionAMemoria + 1)) log_error(logger, "No se pudo conectar al servidor.");
    else close(conexionAMemoria);

    // Generar conexion a CPU    
    logger = cambiarNombre(logger, "Kernel-CPU");
    int conexionACPU = conexion("CPU");
    if(!(conexionACPU + 1)) log_error(logger, "No se pudo conectar al servidor.");
    else close(conexionACPU);

    // Generar conexion a File System
    logger = cambiarNombre(logger, "Kernel-FS");
    int conexionAFS = conexion("FILESYSTEM");
    if(!(conexionAFS + 1)) log_error(logger, "No se pudo conectar al servidor.");
    else close(conexionAFS);

    // Recibir conexiones de consolas
    logger = cambiarNombre(logger, "Kernel-Consola");
    int servidorDeConsola = servirAConsola();
    close(servidorDeConsola);

    terminarPrograma(logger, config);

    return 0; 
}

void terminarPrograma(t_log *logger, t_config *config){
	log_destroy(logger);
    config_destroy(config); 
}

int conexion(char *CONEXION) {
    char *KEYS[] = {
        string_from_format("PUERTO_%s", CONEXION), 
        string_from_format("IP_%s", CONEXION), 
        string_from_format("CLAVE_KERNEL_%s", CONEXION)
    };
    char *puerto = confGet(config, KEYS[0]); 
    free(KEYS[0]);  
    char *ip = confGet(config, KEYS[1]);  
    free(KEYS[1]);  
    char *claveHandshake = confGet(config, KEYS[2]);
    free(KEYS[2]);  
    int conexion = realizarConexion(ip, puerto, claveHandshake); 
    log_info(logger, "Conexion creada: %d", conexion); 
    return conexion;
}