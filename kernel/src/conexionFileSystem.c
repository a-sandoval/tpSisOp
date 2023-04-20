/* KERNEL- cliente | FYLESYSTEM - sevidor*/
#include "kernel/include/conexionFileSystem.h"

int conexionFileSystem(){
    char* puerto = confGet(config,"PUERTO_FILESYSTEM");
    char* ip = confGet(config,"IP_FILESYSTEM"); 
    char* claveHandshake = confGet(config,"CLAVE_KERNEL_FILESYSTEM"); 
    int conexionAFileSystem = realizarConexion(ip, puerto, claveHandshake); 
    log_info(logger, "Conexion creada: %d", conexionAFileSystem); 
   
    return conexionAFileSystem; 
}
