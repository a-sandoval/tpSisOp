/* KERNEL- cliente | MEMORIA - sevidor*/
#include "kernel/include/conexionMemoria.h"

int conexionMemoria() {
    char *puerto = confGet("PUERTO_MEMORIA");
    char *ip = confGet("IP_MEMORIA");
    char *claveHandshake = confGet("CLAVE_KERNEL_MEMORIA"); 
    int conexionAMemoria = realizarConexion(ip, puerto, claveHandshake); 
    log_info(logger, "Conexion creada: %d", conexionAMemoria); 

    return conexionAMemoria; 
}
