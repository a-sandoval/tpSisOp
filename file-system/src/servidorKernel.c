#include "file-system/include/servidorKernel.h"

void escucharAlKernel() {

    char* puertoEscucha;
    char* claveValida; 

    obtenerDeConfiguracionComoServidor(&puertoEscucha, &claveValida); 

    Lista* lista = NULL; 
    insertar(lista,claveValida); 

    int kernel_fd = alistarServidor(logger, puertoEscucha);

    int resultadoEjecucion = ejecutarServidor(kernel_fd, logger, lista);  

    if(resultadoEjecucion == EXIT_FAILURE){
		printf("Doy por finalizado el servidor");
    	borrarLista(lista);
		return 0; 
	}

    borrarLista(lista); 


}


void obtenerDeConfiguracionComoServidor(char ** puertoEscucha, char** claveValida) {

    logger = iniciar_logger("fileSysLog", "Kernel -> File System"); 

    t_config* configServer = iniciarConfiguracion("file-sys.config",logger);

    *puertoEscucha = config_get_string_value(configServer,"PUERTO_ESCUCHA");

    *claveValida = config_get_string_value(configServer,"CLAVE_FS_KERNEL_VALIDA");

    config_destroy(configServer); 

}
