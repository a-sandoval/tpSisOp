#include "conexionMemoria.h"


int conectarseAMemoria() {

   char* puertoConexionAMemoria; 
   char* ip; 
   char* claveHandshake; 

   obtenerDeConfiguracionComoCliente(&puertoConexionAMemoria,&ip,&claveHandshake); 

   int conexionAMemoria = crear_conexion(ip, puertoConexionAMemoria); 

   handshake(claveHandshake,conexionAMemoria); 

   return conexionAMemoria; 
}


void obtenerDeConfiguracionComoCliente(char** puerto, char** ip, char** claveHandshake) {

    t_config* configFileSys = iniciarConfiguracion("file-system.config", logger); 

    *puerto = config_get_string_value(configFileSys,"PUERTO_MEMORIA");

    *ip = config_get_string_value(configFileSys,"IP_MEMORIA"); 

    *claveHandshake = config_get_string_value(configFileSys,"CLAVE_FS_MEMORIA"); 

    config_destroy(configFileSys); 
    
}