#include "fileSystem/include/conexionMemoria.h"


int conectarseAMemoria(t_config* config_fs) {

   char* puertoConexionAMemoria= config_get_string_value(config_fs,"PUERTO_MEMORIA"); 
   char* ip= config_get_string_value(config_fs,"IP_MEMORIA"); 
; 
   char* claveHandshake = config_get_string_value(config_fs,"CLAVE_FS_MEMORIA"); 

   int conexionAMemoria = crear_conexion(ip, puertoConexionAMemoria); 

   handshake(claveHandshake,conexionAMemoria); 

   return conexionAMemoria; 
}


