#include "../include/fileSystem.h"'

t_log* loggerFileSys; 



int main() {

    loggerFileSys = iniciarLogger("fileSys.log","file-system");

    conectarseAMemoria(); 

    escucharAlKernel(); 

    log_info(loggerFileSys,"Se han creado conexiones pertinentes"); // Luego comentar

    log_destroy(loggerFileSys); 

}


void handshake(char*valor,int conexion){
	enviar_mensaje(valor, conexion);
}



