/* - Interprete entre el hardware y el software - */
#include "kernel/include/kernel.h"

int main(){

    /*Inicializando Logger*/
    logger = iniciarLogger("kernel.log", "Kernel");

    /*Inicializando el config*/
    config = iniciarConfiguracion("kernel.config");

    /*
    pthread_t conexionMemoria;
    logger = cambiarNombre("Kernel-Memoria");
    if(!pthread_create(&conexionMemoria, NULL,(void *)conexion("MEMORIA"), NULL)){
        pthread_detach(conexionMemoria);
    }
    else{
        log_error(logger, "Error al inciar conexion a memoria, Abort");
        return EXIT_FAILURE;
    }

     pthread_t conexionCPU;
     int* conexionACPU;
    logger = cambiarNombre("Kernel-CPU");
    if(!pthread_create(&conexionCPU, NULL,(void *) conexion("CPU"), NULL)){
        pthread_detach(conexionCPU);
    }
    else{
        log_error(logger, "Error al inciar conexion a CPU, Abort");
        return EXIT_FAILURE;
    }

    pthread_t conexionFS;
    int* conexionAFS;
    logger = cambiarNombre("Kernel-FS");
    if(!pthread_create(&conexionFS, NULL,(void *) conexion("FILESYSTEM"), NULL)){
        pthread_detach(conexionFS);
    }
    else{
        log_error(logger, "Error al inciar conexion a FS, Abort");
        return EXIT_FAILURE;
    }

    */

    pthread_t servidorConsola;
    logger = cambiarNombre("Kernel-Consola");
    if(!pthread_create(&servidorConsola, NULL,(void *) servirAConsola(), NULL)){
        pthread_detach(servidorConsola);

    }
    else{
        log_error(logger, "Error al inciar servidor Kernel, Abort");
        return EXIT_FAILURE;
    }
   


    terminarPrograma(NULL);

    return 0; 
}



void* conexion(char *CONEXION) {
    char *KEYS[] = {
        string_from_format("PUERTO_%s", CONEXION), 
        string_from_format("IP_%s", CONEXION), 
        string_from_format("CLAVE_KERNEL_%s", CONEXION)
    };
    char *puerto = confGet(KEYS[0]); 
    free(KEYS[0]);  
    char *ip = confGet(KEYS[1]);  
    free(KEYS[1]);  
    char *claveHandshake = confGet(KEYS[2]);
    free(KEYS[2]);  
    int conexion = realizarConexion(ip, puerto, claveHandshake); 
    log_info(logger, "Conexion creada: %d", conexion); 

    close(conexion);

    //return conexion;

}