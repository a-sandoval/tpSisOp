#include "../include/fileSystem.h"'

t_log* loggerFileSys; 
t_config* configFileSys; 


int main() {

    loggerFileSys = iniciarLog("fileSys.log");
    configFileSys = iniciarConfiguracion("file-sys.log"); 

    char* ip = config_get_string_value(configFileSys,"IP_MEMORIA"); 

    // char* ip = obtenerValorDe(configFileSys, "IP_MEMORIA"); otra option

    char* puertoClienteDeMemoria = config_get_string_value(configFileSys,"PUERTO_MEMORIA");

    char* claveHandshakeMem = config_get_string_value(configFileSys,"CLAVE_FS_MEMORIA");  

    char* puertoEscuchaAKernel = config_get_string_value(configFileSys,"PUERTO_ESCUCHA"); 

    int conexion = conectarseAMemoria(ip, puertoClienteDeMemoria, claveHandshakeMem); 

    escucharAlKernel(puertoEscuchaAKernel); 

    log_info(loggerFileSys,"Se han creado conexiones pertinentes"); 

    finalizarModulo(conexion,loggerFileSys,configFileSys); 
}

void iterator(char* value) {
	log_info(logger,"%s", value);
}

void escucharAlKernel(char* puertoDeEscucha) {

    int fsAsServerFD = iniciar_servidor(puertoDeEscucha); 

    log_info(loggerFileSys, "Servidor listo para recibir al cliente");

    int kernelAsClientFD = esperar_cliente(fsAsServerFD);

    t_list* lista;

	while (1) {
		int cod_op = recibir_operacion(kernelAsClientFD);
		switch (cod_op) {
		case MENSAJE:
			recibir_mensaje(kernelAsClientFD);
			break;
		case PAQUETE:
			lista = recibir_paquete(kernelAsClientFD);
			log_info(logger, "Me llegaron los siguientes valores:\n");
			list_iterate(lista, (void*) iterator);
			break;
		case -1:
			log_error(logger, "El Modulo cliente se desconecto. Terminando servidor Memoria");
			return EXIT_FAILURE;
		default:
			log_warning(logger,"Operacion desconocida. No quieras meter la pata");
			break;
		}
	}
    

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

