#include "fileSystem/include/servidorKernel.h"

void escucharAlKernel(t_config* config_fs) {

    char* puertoEscucha= config_get_string_value(config_fs,"PUERTO_ESCUCHA"); 
    char* claveValida = config_get_string_value(config_fs,"CLAVE_FS_KERNEL_VALIDA");

    log_info(logger, "Vamos bien por ahora"); 
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

