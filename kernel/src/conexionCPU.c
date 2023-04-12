/* KERNEL- cliente | CPU - sevidor*/
#include "kernel/include/conexionCPU.h"

int conexionCPU(t_config* config_clienteCPU) {
   char* puertoConexionACPU; 
   char* ip; 
   char* claveHandshake; 
   obtenerDeConfiguracionComoCliente(config_clienteCPU, &puertoConexionACPU,&ip,&claveHandshake);    
   int conexionACPU = crear_conexion(ip, puertoConexionACPU); 
   handshake(claveHandshake,conexionACPU); 
   return conexionACPU; 
}

void obtenerDeConfiguracionComoCliente(t_config* config_clienteCPU, char** puerto, char** ip, char** claveHandshake) {
    *puerto = config_get_string_value(config_clienteCPU,"PUERTO_CPU");
    *ip = config_get_string_value(config_clienteCPU,"IP_CPU"); 
    *claveHandshake = config_get_string_value(config_clienteCPU,"CLAVE_KERNEL_CPU"); 
    config_destroy(config_clienteCPU); 
}