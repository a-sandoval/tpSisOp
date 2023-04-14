#include "fileSystem/include/conexionMemoria.h"


int conectarseAMemoria(t_config* config_fs) {

   char* puertoConexionAMemoria= config_get_string_value(config_fs,"PUERTO_MEMORIA"); 
   char* ip= config_get_string_value(config_fs,"IP_MEMORIA"); 
; 
   char* claveHandshake = config_get_string_value(config_fs,"CLAVE_FS_MEMORIA"); 

   int conexionAMemoria = realizarConexion(ip, puertoConexionAMemoria, claveHandshake); 

   if (conexionAMemoria == -1) {

      log_error(logger,"No se pudo crear la conexion con la memoria");
		
		return EXIT_FAILURE; //abort o exit?

   }

   return conexionAMemoria; 
}


