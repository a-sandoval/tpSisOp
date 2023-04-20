#include "memoria/include/memoria.h"

int main(){

    logger = iniciarLogger("memoria.log", "Memoria");
	config = iniciarConfiguracion("memoria.config", logger);

	Lista* clavesValidas = malloc(sizeof(Lista));
	clavesValidas->cabeza=NULL;
	obtenerClavesValidas(config,clavesValidas);

	//inicio servidor y queda a la espera de clientes
	int cliente_fd = alistarServidor(logger, confGet(config,"PUERTO_ESCUCHA"));

	//int rdoEjecucion = 
	ejecutarServidor(cliente_fd, logger, clavesValidas);

	//if(rdoEjecucion == EXIT_FAILURE) terminarPrograma(config,logger);
	
	log_info(logger,"Terminando servidor");
	close(cliente_fd);
	borrarLista(clavesValidas);

	terminarPrograma(config,logger);
	return 0;
    
}

void iterator(char* value) {
	log_info(logger,"%s", value);
}

void terminarPrograma(t_config* memoriaconfig, t_log* logger){
	log_destroy(logger);
	config_destroy(memoriaconfig);
}

void obtenerClavesValidas(t_config* config, Lista* claves){
	
	insertar(claves, confGet(config, "CLAVE_CPU_MEMORIA"));
	insertar(claves, confGet(config, "CLAVE_KERNEL_MEMORIA"));
	insertar(claves, confGet(config, "CLAVE_FS_MEMORIA"));
}