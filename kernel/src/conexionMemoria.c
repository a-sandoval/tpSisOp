/* KERNEL- cliente | MEMORIA - sevidor*/
#include "kernel/include/conexionMemoria.h"

int conexionMemoria(t_config* config_clienteMemoria) {
   char* puertoConexionAMemoria; 
   char* ip; 
   char* claveHandshake; 
   obtenerDeConfiguracionComoCliente(config_clienteMemoria, &puertoConexionAMemoria,&ip,&claveHandshake);    
   int conexionAMemoria = crear_conexion(ip, puertoConexionAMemoria); 
   handshake(claveHandshake,conexionAMemoria); 
   return conexionAMemoria; 
}

void obtenerDeConfiguracionComoCliente(t_config* config_clienteMemoria, char** puerto, char** ip, char** claveHandshake) {
    *puerto = config_get_string_value(config_clienteMemoria,"PUERTO_MEMORIA");
    *ip = config_get_string_value(config_clienteMemoria,"IP_MEMORIA"); 
    *claveHandshake = config_get_string_value(config_clienteMemoria,"CLAVE_KERNEL_MEMORIA"); 
    config_destroy(config_clienteMemoria); 
}

void terminar_programa(int conexion, t_log* logger){
	log_destroy(logger);
	liberar_conexion(conexion);
}