/* - Interprete entre el hardware y el software - */
#include "kernel/include/kernel.h"

int main(){

    /*Inicializando Logger*/
    logger = iniciarLogger("kernel.log", "Kernel");

    /*Inicializando el config*/
    config = iniciarConfiguracion("kernel.config");

    // Generar conexion a memoria
    logger = cambiarNombre("Kernel-Memoria");
    int conexionAMemoria = conexion("MEMORIA");
    if(!(conexionAMemoria + 1))
        log_error(logger, "No se pudo conectar al servidor.");   
    else
        close(conexionAMemoria);

    // Generar conexion a CPU    
    logger = cambiarNombre("Kernel-CPU");
    int conexionACPU = conexion("CPU");
    if(!(conexionACPU + 1))
        log_error(logger, "No se pudo conectar al servidor.");
    else
        close(conexionACPU);

    // Generar conexion a File System
    logger = cambiarNombre("Kernel-FS");
    int conexionAFS = conexion("FILESYSTEM");
    if(!(conexionAFS + 1))
        log_error(logger, "No se pudo conectar al servidor.");
    else
        close(conexionAFS);

    // Recibir conexiones de consolas
    logger = cambiarNombre("Kernel-Consola");
    int servidorDeConsola = servirAConsola();
    close(servidorDeConsola);


    terminarPrograma(NULL);

    return 0; 
}



int conexion(char *CONEXION) {
    char *KEYS[] = {
        string_from_format("PUERTO_%s", CONEXION), 
        string_from_format("IP_%s", CONEXION), 
        string_from_format("CLAVE_KERNEL_%s", CONEXION)
    };
    char *puerto = confGet(KEYS[0]); 
    free(KEYS[0]);  
    char *ip = confGet(KEYS[1]);  
    free(KEYS[1]);  
    char *claveHandshake = confGet(KEYS[2]);
    free(KEYS[2]);  
    int conexion = realizarConexion(ip, puerto, claveHandshake); 
    log_info(logger, "Conexion creada: %d", conexion); 
    return conexion;
}