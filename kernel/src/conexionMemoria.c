/* KERNEL- cliente | MEMORIA - sevidor*/
#include "../include/conexionMemoria.h"

int conexionMemoria(t_config* config_clienteMemoria) {
   char *puertoConexionAMemoria = string_new(); 
   char *ip = string_new();
   char *claveHandshake = string_new(); 

   obtenerDeConfiguracionComoClienteMemoria(config_clienteMemoria, &puertoConexionAMemoria,&ip,&claveHandshake);  
   printf("%s %s %s \n", puertoConexionAMemoria, ip, claveHandshake);
   int conexionAMemoria = crear_conexion(ip, puertoConexionAMemoria); 
   
   handshake(claveHandshake,conexionAMemoria); 
   return conexionAMemoria; 
}

void obtenerDeConfiguracionComoClienteMemoria(t_config* config_clienteMemoria, char** puerto, char** ip, char** claveHandshake) {
    char *temp1 = config_get_string_value(config_clienteMemoria,"PUERTO_MEMORIA");
    char *temp2 = config_get_string_value(config_clienteMemoria,"IP_MEMORIA");
    char *temp3 = config_get_string_value(config_clienteMemoria,"CLAVE_KERNEL_MEMORIA"); 

    *puerto = strdup(temp1);
    *ip =  strdup(temp2);
    *claveHandshake = strdup(temp3);
    //free(temp1); free(temp2); free(temp3);
    printf("%s %s %s \n", *puerto, *ip, *claveHandshake);
    config_destroy(config_clienteMemoria); 
}

void terminar_programaMemoria(int conexion, t_log* logger){
	log_destroy(logger);
	liberar_conexion(conexion);
}