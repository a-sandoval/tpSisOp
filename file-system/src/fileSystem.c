#include "../include/fileSystem.h"'

t_log* loggerFileSys; 



int main() {

    loggerFileSys = iniciarLogger("fileSys.log","file-system");



    

    char* claveHandshakeMem = config_get_string_value(configFileSys,"CLAVE_FS_MEMORIA");  

    char* puertoEscuchaAKernel = config_get_string_value(configFileSys,"PUERTO_ESCUCHA"); 

    //int conexionMemoria = conectarseAMemoria(ip, puertoClienteDeMemoria); 

    escucharAlKernel(puertoEscuchaAKernel); 

    log_info(loggerFileSys,"Se han creado conexiones pertinentes"); // Luego comentar

    finalizarModulo(conexionMemoria,loggerFileSys,configFileSys); 

}


void escucharAlKernel(char* puertoDeEscucha) {

    int fsAsServerFD = iniciar_servidor(puertoDeEscucha); 

    log_info(loggerFileSys, "Servidor listo para recibir al cliente");

    int kernelAsClientFD = esperar_cliente(fsAsServerFD);


}






void handshake(char*valor,int conexion){
	enviar_mensaje(valor, conexion);
}



// FINALIZAR MODULO
void finalizarModulo(int conexion, t_log* logger, t_config* config)
{
	log_destroy(logger); 
	config_destroy(config); 
	liberar_conexion(conexion); 
}

