#include "../include/fileSystem.h"

int main() {

    t_log* loggerFileSys = iniciarLog("fileSys.log");
    t_config* configFileSys = iniciarConfiguracion("file-sys.log"); 

    char* ip = config_get_string_value(configFileSys,"IP_MEMORIA"); 

    // char* ip = obtenerValorDe(configFileSys, "IP_MEMORIA"); otra option

    char* puertoClienteDeMemoria = config_get_string_value(configFileSys,"PUERTO_MEMORIA");

    char* claveHandshakeMem = config_get_string_value(configFileSys,"CLAVE_FS_MEMORIA");  

    char* puertoEscuchaAKernel = config_get_string_value(configFileSys,"PUERTO_ESCUCHA"); 

    int conexion = conectarseAMemoria(ip, puertoClienteDeMemoria, claveHandshakeMem); 

    escucharAlKernel(puertoEscuchaAKernel); 

    finalizarModulo(conexion,loggerFileSys,configFileSys); 
}


t_config* iniciarConfiguracion(char* ruta) {

    t_config* nuevo_config=config_create(ruta); 

    if(nuevo_config==NULL) {
		log_info(logger,"Error al generar archivo de config"); 
		log_destroy(logger); 
		return EXIT_FAILURE; 
	}

    return nuevo_config; 
}

t_log* iniciarLog(char* ruta) {

    t_log* logger = log_create(ruta,"loggeandoAndo",1,LOG_LEVEL_INFO); 

    log_info(logger,"Listo mi log"); // borrar en algun momento

    if (logger==NULL) {

		printf("Error al generar logger"); 
		return EXIT_FAILURE;
	}

    return logger; 
}


int conectarseAMemoria(char* ip, char* puerto, char* clave) {


    int conexion = crear_conexion(ip, puerto); 

    handshake(clave,conexion); 

    return conexion; 
}

void handshake(char*valor,int conexion){
	enviar_mensaje(valor, conexion);
}
char* obtenerValorDe(t_config* configuracion, char* key) {

    return config_get_string_value(configuracion,key); 
}

void finalizarModulo(int conexion, t_log* logger, t_config* config)
{
	
	log_destroy(logger); 
	config_destroy(config); 
	liberar_conexion(conexion); 
}

int crear_conexion(char *ip, char* puerto) {
	struct addrinfo hints;
	struct addrinfo *server_info;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(ip, puerto, &hints, &server_info);

	int socket_cliente = socket(server_info->ai_family,
                			    server_info->ai_socktype,
                			    server_info->ai_protocol);

	connect(socket_cliente, server_info->ai_addr, server_info->ai_addrlen);
	freeaddrinfo(server_info);
	return socket_cliente;
}

void liberar_conexion(int socket_cliente) {
	close(socket_cliente);
}