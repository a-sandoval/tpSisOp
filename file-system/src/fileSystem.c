#include "../include/fileSystem.h"




int main() {
    loggerFileSys = iniciar_logger("fileSys.log","file-system");
    t_config* config_fs = config_create("file-system.config"); 

    conectarseAMemoria(); 

    escucharAlKernel(); 

    log_info(loggerFileSys,"Se han creado conexiones pertinentes"); // Luego comentar

    log_destroy(loggerFileSys); 

    config_destroy(config_fs); 

}

Lista* obtenerClavesValidas(t_config* config_fs){

    Lista* lista=NULL; 

    insertar(lista,config_get_string_value(config_fs, "CLAVE_FS_KERNEL_VALIDA")); 

	return lista; 
}





