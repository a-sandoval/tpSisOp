/* KERNEL- cliente | FYLESYSTEM - sevidor*/
#include "kernel/include/conexionFileSystem.h"

int conexionFileSystem(t_config* config_clienteFileSystem){
   char* puertoConexionAFileSystem; 
   char* ip; 
   char* claveHandshake; 
   obtenerDeConfiguracionComoCliente(config_clienteFileSystem, &puertoConexionAFileSystem,&ip,&claveHandshake);    
   int conexionAFileSystem = crear_conexion(ip, puertoConexionAFileSystem); 
   handshake(claveHandshake,conexionAFileSystem); 
   return conexionAFileSystem; 
}

void obtenerDeConfiguracionComoCliente(t_config* config_clienteFileSystem, char** puerto, char** ip, char** claveHandshake) {
    *puerto = config_get_string_value(config_clienteFileSystem,"PUERTO_FILESYSTEM");
    *ip = config_get_string_value(config_clienteFileSystem,"IP_FILESYSTEM"); 
    *claveHandshake = config_get_string_value(config_clienteFileSystem,"CLAVE_KERNEL_FILESYSTEM"); 
    config_destroy(config_clienteFileSystem); 
}

/*
void terminar_programa(int conexion, t_log* logger){
	log_destroy(logger);
	liberar_conexion(conexion);
}*/