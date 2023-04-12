#include "conexionMemoria.h"


int conectarseAMemoria() {

    t_config* configFileSys = iniciarConfiguracion("file-system.config", logger); 

    char* puertoClienteDeMemoria = config_get_string_value(configFileSys,"PUERTO_MEMORIA");

    char* ip = config_get_string_value(configFileSys,"IP_MEMORIA"); 

    //int conexion = crear_conexion(ip, puerto); 

    //handshake(clave,conexion); 

    return conexion; 
}