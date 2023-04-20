#include "fileSystem/include/conexionMemoria.h"

int conectarseAMemoria() {

   char* puertoConexionAMemoria = confGet(config,"PUERTO_MEMORIA"); 
   char* ip = confGet(config,"IP_MEMORIA"); 
   char* claveHandshake = confGet(config,"CLAVE_FS_MEMORIA"); 

   int conexionAMemoria = realizarConexion(ip, puertoConexionAMemoria, claveHandshake); 

   if (conexionAMemoria == -1) {

      log_error(logger,"No se pudo crear la conexion con la memoria");
		
		return EXIT_FAILURE; //abort o exit?

   }

   return conexionAMemoria; 
}


