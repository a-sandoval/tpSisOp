/* KERNEL- cliente | CPU - sevidor*/
#include "kernel/include/conexionCPU.h"

int conexionCPU(t_config* config_clienteCPU) {
   char* puertoConexionACPU = config_get_string_value(config_clienteCPU,"PUERTO_CPU"); 
   char* ip = config_get_string_value(config_clienteCPU,"IP_CPU");  
   char* claveHandshake = config_get_string_value(config_clienteCPU,"CLAVE_KERNEL_CPU"); 

   int conexionACPU = crear_conexion(ip, puertoConexionACPU); 

   handshake(claveHandshake,conexionACPU); 

   return conexionACPU; 
}


