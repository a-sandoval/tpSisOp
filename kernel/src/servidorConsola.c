/* CONSOLA - cliente | KERNEL - servidor*/
#include "kernel/include/servidorConsola.h"

t_pcb* PCB; 

int servirAConsola(){


	char* puertoDeEscucha = confGet("PUERTO_ESCUCHA"); 
	sem_init(&hayProcesosNuevos,0,0);
	sem_init(&hayProcesosReady,0,0); 
	pthread_mutex_init(&mutexListaNew, NULL);


	inicializarListasPCBS(); 

	pthread_t recibirConsolas; // Hilo Principal -> Recibe consolas y crea PCBs 
    	if(!pthread_create(&recibirConsolas, NULL,(void *) recibirConsolas, &puertoDeEscucha)){
    	    pthread_join(recibirConsolas,NULL);
    	}
    	else{
    	    log_error(logger, "Error al iniciar servidor Kernel, Abort");
    	    return EXIT_FAILURE;
    	}

	pthread_t planificadorLargoPlazo; //Hilo Planificador Largo Plazo -> Mueve procesos de NEW a READY
    	if(!pthread_create(&planificadorLargoPlazo, NULL,(void *) planificarALargoPlazo, NULL)){
    	    pthread_detach(planificadorLargoPlazo);
    	}
    	else{
    	    log_error(logger, "Error al inciar servidor Kernel, Abort");
    	    return EXIT_FAILURE;
		}

	pthread_t planificadorCortoPlazo;  //Hilo Planificador Corto Plazo --> Mueve procesos de READY a EXEC
		if(!pthread_create(&planificadorCortoPlazo, NULL,(void *) planificarACortoPlazo, NULL)){
    	    pthread_detach(planificadorCortoPlazo);
    	}
    	else{
    	    log_error(logger, "Error al inciar servidor Kernel, Abort");
    	    return EXIT_FAILURE;
		}

	return 0;
}



void recibirConsolas(char *puerto){ 
	
	int server_fd = iniciarServidor(puerto);

	while(1){

		socketCliente = esperar_cliente(server_fd);	
	
		ejecutarServidorKernel(); 
	
		sem_post(&hayProcesosNuevos);
	}

}

void iterator(void *value){
	list_add(PCB->instrucciones, value);
}

void ejecutarServidorKernel(){
	
	t_list* lista;
	
	PCB = crearPCB(); 
	PCB->socketPCB=socketCliente; 


	while (1) {
		int cod_op = recibir_operacion(socketCliente); 
		switch (cod_op) {
		case MENSAJE:
			
			break;
		case PAQUETE:
			lista = recibir_paquete(socketCliente);
			//log_info(logger, "Me llegaron los siguientes valores:"); 
			list_iterate(lista, (void*) iterator); 
			list_destroy_and_destroy_elements(lista, (void*)element_destroyer);
			break;
		default:
			break;
		}
	}

	ingresarANew(PCB); 
}

