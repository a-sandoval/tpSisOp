/* KERNEL- cliente | FYLESYSTEM - sevidor*/
#include "kernel/include/conexionFileSystem.h"

int conexionFileSystem(t_config* config_clienteFileSystem){
    char* puerto = config_get_string_value(config_clienteFileSystem,"PUERTO_FILESYSTEM");
    char* ip = config_get_string_value(config_clienteFileSystem,"IP_FILESYSTEM"); 
    char* claveHandshake = config_get_string_value(config_clienteFileSystem,"CLAVE_KERNEL_FILESYSTEM"); 
   
   int conexionAFileSystem = crear_conexion(ip, puerto); 
   handshake(claveHandshake,conexionAFileSystem); 

   
   return conexionAFileSystem; 
}
