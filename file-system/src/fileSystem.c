#include "fileSystem.h"




int main() {

    loggerFileSys = iniciarLogger("fileSys.log","file-system");

    conectarseAMemoria(); 

    escucharAlKernel(); 

    log_info(loggerFileSys,"Se han creado conexiones pertinentes"); // Luego comentar

    log_destroy(loggerFileSys); 

}




