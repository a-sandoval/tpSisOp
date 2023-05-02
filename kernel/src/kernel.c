/* - Interprete entre el hardware y el software - */
#include "kernel/include/kernel.h"

int main(){


    /*Inicializando Logger*/
    logger = iniciarLogger("kernel.log", "Kernel");

    /*Inicializando el config*/
    config = iniciarConfiguracion("kernel.config");
    
    /*
    // Generar conexion a memoria
        conexionMemoria();
   

    // Generar conexion a CPU  
      conexionCpu();
    

    // Generar conexion a File System
        conexionFileSystem();
    

    */

    //logger = cambiarNombre("Kernel-Consola\0");
    int servidorDeConsola = servirAConsola();
    close(servidorDeConsola);

    terminarPrograma();

    return 0; 
}