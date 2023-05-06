/* - Interprete entre el hardware y el software - */
#include "kernel/include/kernel.h"

int main(){
    //Inicializar variables
    logger = iniciarLogger("kernel.log", "Kernel");
    config = iniciarConfiguracion("kernel.config");
	gradoMultiprogramacion = obtenerGradoMultiprogramacion();
	char* puertoDeEscucha = confGet("PUERTO_ESCUCHA"); 
	inicializarSemaforos();
	inicializarListasPCBS(); 
	
	conexionCPU();
	log_info(logger, "Conectado al CPU");

    //Inicializar Hilos
	pthread_t planificadorLargoPlazo_h; //Hilo Planificador Largo Plazo -> Mueve procesos de NEW a READY
    	if(!pthread_create(&planificadorLargoPlazo_h, NULL,(void *) planificarALargoPlazo, NULL))
    	    pthread_detach(planificadorLargoPlazo_h);
    	else{
    	    log_error(logger, "Error al inciar servidor Kernel, Abort");
    	    return EXIT_FAILURE;
		}

	pthread_t planificadorCortoPlazo_h;  //Hilo Planificador Corto Plazo --> Mueve procesos de READY a EXEC
		if(!pthread_create(&planificadorCortoPlazo_h, NULL, (void*) planificarACortoPlazoSegunAlgoritmo, NULL))
    	    pthread_detach(planificadorCortoPlazo_h);
    	else{
    	    log_error(logger, "Error al inciar servidor Kernel, Abort");
    	    return EXIT_FAILURE;
		}

	pthread_t recibirConsolas_h; // Hilo Principal -> Recibe consolas y crea PCBs 
    	if(!pthread_create(&recibirConsolas_h, NULL,(void *) recibirConsolas, puertoDeEscucha)) 
            pthread_join(recibirConsolas_h, NULL);
    	else{
    	    log_error(logger, "Error al iniciar servidor Kernel, Abort");
    	    return EXIT_FAILURE;
    	}

    terminarPrograma();

    return 0; 
}