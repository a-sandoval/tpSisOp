/* KERNEL- cliente | MEMORIA - sevidor*/
#include "../include/conexionMemoria.h"


int conexionMemoria(t_config* config_clienteMemoria) {
    char *puerto = config_get_string_value(config_clienteMemoria,"PUERTO_MEMORIA");
    char *ip = config_get_string_value(config_clienteMemoria,"IP_MEMORIA");
    char *claveHandshake = config_get_string_value(config_clienteMemoria,"CLAVE_KERNEL_MEMORIA"); 
    int conexionAMemoria = crear_conexion(ip, puerto); 
   
    handshake(claveHandshake,conexionAMemoria); 

    return conexionAMemoria; 
}
