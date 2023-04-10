#include "../include/fileSystem.h"

int main() {

    t_log* loggerFileSys = iniciarLog("fileSys.log");
    t_config* configFileSys = iniciarConfiguracion("file-sys.log"); 

    char* ip =

    iniciarModulo();

    finalizarModulo(); 
}

void iniciarModulo() {

    levantarConfiguracion(); 

    conectarseAMemoria();

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

    log_info(logger,"Listo mi log"); 

    if (logger==NULL) {

		printf("Error al generar logger"); 
		return EXIT_FAILURE;
	}

    return logger; 
}


void conectarseAMemoria() {

    

}