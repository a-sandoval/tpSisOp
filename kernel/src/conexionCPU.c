/* KERNEL- cliente | CPU - sevidor*/
#include "kernel/include/conexionCPU.h"

int conexionCPU() {
   char* puerto = confGet("PUERTO_CPU"); 
   char* ip = confGet("IP_CPU");  
   char* claveHandshake = confGet("CLAVE_KERNEL_CPU"); 
   int conexionACPU = realizarConexion(ip, puerto, claveHandshake); 
   log_info(logger, "Conexion creada: %d", conexionACPU); 
   
   return conexionACPU; 
}


