#include "memoria/include/memoria.h"



int main(){

	t_config* memoria_config;
    logger = iniciar_logger("memoria.log", "Memoria");
	memoria_config = iniciarConfiguracion("memoria.config", logger);

	Lista* clavesValidas = malloc(sizeof(Lista));
	clavesValidas->cabeza=NULL;
	//obtenerClavesValidas(memoria_config); ORIGINAL
	obtenerClavesValidas(memoria_config,clavesValidas);

	//inicio servidor y queda a la espera de clientes
	int cliente_fd = alistarServidor(logger, config_get_string_value(memoria_config,"PUERTO_ESCUCHA"));

	//int rdoEjecucion = 
	ejecutarServidor(cliente_fd,logger, clavesValidas);

	//if(rdoEjecucion == EXIT_FAILURE){
	//terminar_programa(memoria_config,logger);
	
	config_destroy(memoria_config); 
	log_info(logger,"Terminando servidor");
	log_destroy(logger);
	//printf("tuki");
	borrarLista(clavesValidas);

	/* 
	
	█▄▄ █░░ █▀█ █▀█ █░█ █▀▀   █▀ █▀█ █▀█ █▀█ █▀█ █▀▀ █▀ ▄▀█   ▀ █▀█
	█▄█ █▄▄ █▄█ ▀▀█ █▄█ ██▄   ▄█ █▄█ █▀▄ █▀▀ █▀▄ ██▄ ▄█ █▀█   ▄ █▄█

	pthread_t hilo[3];
	for(int i = 0; i < 3; i++) {
		pthread_create(&hilo[i], NULL, procesoPrincipal, string_from_format("Hilo: %d", i+1));
		usleep(1000);
	}
		pthread_join(hilo[0], NULL);
		pthread_join(hilo[1], NULL);
		pthread_join(hilo[2], NULL);
	*/
	return 0;
    
}
/*
void *procesoPrincipal(void *thread_name) {

	return NULL;

}
*/
void iterator(char* value) {
	log_info(logger,"%s", value);
}

void terminar_programa(t_config* memoriaconfig, t_log* logger){
	log_destroy(logger);
	config_destroy(memoriaconfig);
}

void obtenerClavesValidas(t_config* memoria_config,Lista* claves){
	
	insertar(claves, config_get_string_value(memoria_config, "CLAVE_CPU_MEMORIA"));
	insertar(claves, config_get_string_value(memoria_config, "CLAVE_KERNEL_MEMORIA"));
	insertar(claves, config_get_string_value(memoria_config, "CLAVE_FS_MEMORIA"));
}
