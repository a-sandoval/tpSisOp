/* - Interprete entre el hardware y el software - */
#include "kernel/include/kernel.h"


int socketCliente;
t_log* logger;
t_log* loggerError;
t_config* config;
pthread_t planificadorLargoPlazo_h, planificadorCortoPlazo_h, recibirConsolas_h;


int main () {
    //Inicializar variables
    logger = iniciarLogger("kernel.log", "Kernel");
	loggerError = iniciarLogger("errores.log", "Errores - Kernel"); 
    config = iniciarConfiguracion("kernel.config");
	atexit (terminarPrograma);
	char* puertoDeEscucha = confGet("PUERTO_ESCUCHA"); 

	inicializarSemaforos();
	atexit (destruirSemaforos);
	inicializarListasPCBs(); 
	atexit (destruirListasPCBs);
	
	conexionCPU();
	conexionFileSystem(); 
	conexionMemoria(); 

    //Inicializar Hilos
	//Hilo Planificador Largo Plazo -> Mueve procesos de NEW a READY
    if (!pthread_create(&planificadorLargoPlazo_h, NULL, (void *) planificarALargoPlazo, NULL)) 
        pthread_detach(planificadorLargoPlazo_h);
    else 
        {error ("Error al generar hilo para el planificador de largo plazo, terminando el programa.");}
	

	//Hilo Planificador Corto Plazo --> Mueve procesos de READY a EXEC
	if (!pthread_create(&planificadorCortoPlazo_h, NULL, (void*) planificarACortoPlazoSegunAlgoritmo, NULL)) 
		pthread_detach(planificadorCortoPlazo_h);
	
    else 
        error ("Error al generar hilo para el planificador de corto plazo, terminando el programa.");
	
	
	// Hilo Principal -> Recibe consolas y crea PCBs 
    if (!pthread_create(&recibirConsolas_h, NULL,(void *) recibirConsolas, puertoDeEscucha)) 
           pthread_join(recibirConsolas_h, NULL);
    else
		error ("Error al generar hilo para recibir consolas, terminando el programa.");

    exit (0);
}
