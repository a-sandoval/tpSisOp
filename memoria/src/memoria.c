#include "memoria/include/memoria.h"

int sockets[3];
pthread_t threadCPU, threadFS, threadKernel;

int main() {

    logger = iniciarLogger ("memoria.log", "Memoria");
	loggerError = iniciarLogger ("memoriaErrores.log","Memoria (Errores)"); 
	config = iniciarConfiguracion ("memoria.config");

	atexit (terminarPrograma);

	creacionEspacio();
	creacionListaHuecosLibres(); 

	//Inicio servidor y queda a la espera de las conexiones de CPU, Kernel y File System
	
	int server_fd = iniciarServidor (confGet("PUERTO_ESCUCHA"));

	log_info (logger, "Servidor listo para recibir clientes");
	log_info (logger, "Esperando CPU...");

	sockets[0] = esperarCliente(server_fd);
	
    if (!pthread_create (&threadCPU, NULL, (void*) ejecutarServidorCPU, (void*) &sockets[0])){
		pthread_detach (threadCPU);
	}	
    else {
        error ("Error en iniciar el servidor a CPU");
    }
	
	usleep (1000 * 500);
	log_info (logger, "Esperando File System...");
	sockets[1] = esperarCliente (server_fd);

    if(!pthread_create (&threadFS, NULL, (void*)ejecutarServidorFS, (void*) &sockets[1])){
		pthread_detach (threadFS);
	}	
    else {
        error ("Error en iniciar el servidor a FS");
    }
	
	usleep (1000 * 500);
	log_info (logger, "Esperando Kernel...");

	sockets[2] = esperarCliente (server_fd);

    if(!pthread_create (&threadKernel, NULL, (void*) ejecutarServidorKernel, (void*) &sockets[2])){
		pthread_join (threadKernel, NULL);
	}	
    else {
        error ("Error en iniciar el servidor a Kernel");
    }

	exit (0);
}
