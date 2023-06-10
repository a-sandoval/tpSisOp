#include "memoria/include/memoria.h"

int sockets[3];
t_segmento * segmento0; 
pthread_t threadCPU, threadFS, threadKernel;

int main() {

    logger = iniciarLogger ("memoria.log", "Memoria");
	loggerError = iniciarLogger ("memoriaErrores.log","Memoria (Errores)"); 
	config = iniciarConfiguracion ("memoria.config");

	atexit (terminarPrograma);

	espacioDeUsuario = malloc (config_get_int_value(config,"TAM_MEMORIA")); 
	if (espacioDeUsuario == NULL) error ("No se pudo alocar memoria al espacio de usuario.");
	atexit (liberarEspacioDeUsuario);
	segmento0 = crearSegmentoCompartido (); 
	atexit (liberarSegmentoCompartido);

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


t_segmento* crearSegmentoCompartido() {

	segmento0 = malloc(sizeof(t_segmento)); 
    segmento0->id=0; 
    segmento0->direccionBase=0; 
    segmento0->tamanio = config_get_int_value(config,"TAM_SEGMENTO_0"); 

	return segmento0; 
}

void liberarEspacioDeUsuario () {
	free (espacioDeUsuario);
}

void liberarSegmentoCompartido () {
	free (segmento0);
}
