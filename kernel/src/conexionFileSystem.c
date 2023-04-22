/* KERNEL- cliente | FYLESYSTEM - sevidor*/
#include "kernel/include/conexionFileSystem.h"

int conexionFileSystem(){
    char* puerto = confGet("PUERTO_FILESYSTEM");
    char* ip = confGet("IP_FILESYSTEM"); 
    char* claveHandshake = confGet("CLAVE_KERNEL_FILESYSTEM"); 
    int conexionAFileSystem = realizarConexion(ip, puerto, claveHandshake); 
    log_info(logger, "Conexion creada: %d", conexionAFileSystem); 
   
    return conexionAFileSystem; 
}
