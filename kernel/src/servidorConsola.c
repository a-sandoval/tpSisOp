/* CONSOLA - cliente | KERNEL - servidor*/
#include "kernel/include/servidorConsola.h"

t_queue* queueConexiones; 
t_pcb* PCB; 

int servirAConsola(){

	signal(SIGINT, agarrarSIGINT);

	char* puertoDeEscucha = confGet("PUERTO_ESCUCHA"); 
	
	clavesValidas = list_create();
	queueConexiones = queue_create(); 

	
	while(!pararPrograma) {
		//inicio servidor y queda a la espera de clientes

		pthread_t recibirConsolas;
    	if(!pthread_create(&recibirConsolas, NULL,(void *) alistarServidorKernel, &puertoDeEscucha)){
    	    pthread_detach(recibirConsolas);
    	}
    	else{
    	    log_error(logger, "Error al inciar servidor Kernel, Abort");
    	    return EXIT_FAILURE;
    	}


		pthread_t newProcesos;
    	if(!pthread_create(&newProcesos, NULL,(void *) ejecutarServidor, NULL)){
    	    pthread_detach(newProcesos);
    	}
    	else{
    	    log_error(logger, "Error al inciar servidor Kernel, Abort");
    	    return EXIT_FAILURE;
    	}
	}
	

	return 0;
}



void agarrarSIGINT(int SIGNUM) {
	pararPrograma = 1;
}

void alistarServidorKernel(char *puerto){

	while(1){
		
	int server_fd = iniciar_servidor(puerto);

	log_info(logger, "Servidor listo para recibir al cliente");

	int socketClienteFD = esperar_cliente(server_fd);

	queue_push(queueConexiones, socketClienteFD);

	// cada vez que carga un proceso deberia hacer signal no?
	//sem_post(hayProcesos);

	}

}

void iterator(void *value){
    
	list_add(PCB->instrucciones, value);


}

int ejecutarServidorKernel(){
	
	t_list* lista;

	//se que esto esta mal porque le tiene que mandar la señal otra funcion y no va declarado aca
	sem_t hayProcesos; 
	sem_init(&hayProcesos,0,0); //no se si la 2da variable esta bien

	sem_wait(&hayProcesos);
	int socketCliente;
	socketCliente = queue_pop(queueConexiones);
	PCB = crearPCB(); 

	
	int cod_op = 0;
	while (cod_op != -1) {
		cod_op = recibir_operacion(socketCliente); 
		switch (cod_op) {
		case MENSAJE:
			char *claveRecibida = recibir_clave(socketCliente);
			log_info(logger, "Se conecto %s", claveRecibida);
			free(claveRecibida); 
			break;
		case PAQUETE:
			lista = recibir_paquete(socketClienteFD);
			//log_info(logger, "Me llegaron los siguientes valores:"); 
			list_iterate(lista, (void*) iterator); 
			list_destroy_and_destroy_elements(lista, (void*)element_destroyer);
			break;
		case -1:
			log_error(logger, "El cliente se desconecto");
			break;
		default:
			log_warning(logger,"Operacion desconocida. No quieras meter la pata");
			break;
		}
	}

	// suponiendo un PCB con instrucciones
	encolar(*pcbsNEW, PCB);
}

