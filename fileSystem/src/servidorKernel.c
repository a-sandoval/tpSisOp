#include "fileSystem/include/servidorKernel.h"

void escucharAlKernel() {

    char* puertoEscucha;
    char* claveValida; 

    obtenerDeConfiguracionComoServidor(&puertoEscucha, &claveValida); 

    Lista* listaClavesValidas = malloc(sizeof(Lista)); 
    listaClavesValidas-> cabeza = NULL; 

    insertar(listaClavesValidas,claveValida); 

    int kernel_fd = alistarServidor(logger, puertoEscucha);

    int resultadoEjecucion = ejecutarServidor(kernel_fd, logger, listaClavesValidas);  

    if(resultadoEjecucion == EXIT_FAILURE){
		printf("Doy por finalizado el servidor");
    	borrarLista(listaClavesValidas);
	
	}

    borrarLista(listaClavesValidas); 


}


void obtenerDeConfiguracionComoServidor(char ** puertoEscucha, char** claveValida) {

    t_config* configServer = iniciarConfiguracion("file-sys.config",logger);

    char* puerto = config_get_string_value(configServer,"PUERTO_ESCUCHA"); 

    *puertoEscucha=malloc(sizeof(strlen(puerto)+1));
    
    strcpy(*puertoEscucha, puerto); 

    logger = iniciar_logger("fileSysLog", "Kernel -> File System"); 


    *puertoEscucha = config_get_string_value(configServer,"PUERTO_ESCUCHA");

    *claveValida = config_get_string_value(configServer,"CLAVE_FS_KERNEL_VALIDA");

    config_destroy(configServer); 

}
