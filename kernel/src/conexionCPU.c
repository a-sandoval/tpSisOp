/* KERNEL- cliente | CPU - sevidor*/
#include "kernel/include/conexionCPU.h"

int conexionCPU() {
   char* puerto = confGet(config,"PUERTO_CPU"); 
   char* ip = confGet(config,"IP_CPU");  
   char* claveHandshake = confGet(config,"CLAVE_KERNEL_CPU"); 
   int conexionACPU = realizarConexion(ip, puerto, claveHandshake); 
   log_info(logger, "Conexion creada: %d", conexionACPU); 
   
   return conexionACPU; 
}


