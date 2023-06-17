#include "memoria/include/conexionKernel.h"

int ejecutarServidorKernel(int *socketCliente){
	
	log_debug(logger, "Conectado el Kernel");
	
	while (1) {
		int peticion = recibirOperacion(*socketCliente);
		
		log_info(logger,"Se recibio peticion %d del Kernel", peticion); 

		switch (peticion) {
			case NEWPCB:
				t_proceso* procesoNuevo = crearProcesoEnMemoria(recibirPID(*socketCliente)); 
				enviarTablaSegmentos(procesoNuevo);
				break;
            case ENDPCB:
				uint32_t pid = recibirPID(*socketCliente);
				//Primero tendriamos que buscar y borrar todos los segmentos que guardo este pid, actualizar huecos libres
				list_remove_and_destroy_element(tablaDeTablasDeSegmentos,pid,(void*)eliminarProcesoDeMemoria); 
				log_info(logger, "Eliminación de Proceso PID: %d",pid);
				break;
			case CREATE_SEGMENT_OP:
				t_peticion* peticion = recibirPeticionCreacionDeSegmento(*socketCliente); 
				ubicarSegmentosEnEspaciosLibres(peticion); 
				break;
            case DELETE_SEGMENT_OP:
                log_info(logger, "Borro segmento dado");
				break;
			case -1:
				log_error(logger, "El Kernel se desconecto");
				return EXIT_FAILURE;
				break; 
			default:
				log_warning(logger,"Operacion desconocida. No quieras meter la pata");
				break;
		}
	}
}


t_list* crearTablaDeSegmentosInicial() {

    t_list* tablaDeSegmentos = list_create(); 
    list_add(tablaDeSegmentos,(void*)segmento0);

    return tablaDeSegmentos;  

}

t_proceso* crearProcesoEnMemoria(uint32_t pid) {

	t_proceso* procesoNuevo = malloc(sizeof(t_proceso)); 
	procesoNuevo->pid = pid; 
	procesoNuevo->tablaDeSegmentosAsociada = crearTablaDeSegmentosInicial();
	list_add(tablaDeTablasDeSegmentos,(void*)procesoNuevo);
	log_info(logger, "Creacion de Proceso PID: %d", procesoNuevo->pid);

	return procesoNuevo;
}

uint32_t recibirPID(int socketCliente) {

	int size, desplazamiento=0; 
	uint32_t pid; 

	void* buffer = recibirBuffer(socketCliente, &size);
	desplazamiento += sizeof(uint32_t);
	memcpy(&(pid), buffer + desplazamiento, sizeof(uint32_t));

	return pid; 

}

//serializar tabla de Segmentos

void enviarTablaSegmentos(t_proceso* procesoEnMemoria){ 
    t_paquete* paquete = crearPaquete();
    
    paquete->codigo_operacion = TABLADESEGMENTOS;

    uint32_t tablaDeSegmentosSize = list_size(procesoEnMemoria->tablaDeSegmentosAsociada);

	agregarAPaquete(paquete,(void*)&procesoEnMemoria->pid,sizeof(uint32_t)); 
   
    uint32_t i;
    for(i=0;i<tablaDeSegmentosSize;i++){
        agregarSegmentoAPaquete(paquete,list_get(procesoEnMemoria->tablaDeSegmentosAsociada, i));
    }

    enviarPaquete(paquete, sockets[0]);

	eliminarPaquete(paquete);
}

void agregarSegmentoAPaquete(t_paquete* paquete, t_segmento* segmento){
	agregarAPaquete(paquete, (void*)&segmento->id, sizeof(uint32_t));
	agregarAPaquete(paquete, (void*)&segmento->direccionBase, sizeof(uint32_t));
	agregarAPaquete(paquete, (void*)&segmento->tamanio, sizeof(uint32_t));
}

void eliminarProcesoDeMemoria(t_proceso* proceso) {
	list_remove_element(proceso->tablaDeSegmentosAsociada, (void*)segmento0); 
	list_destroy_and_destroy_elements(proceso->tablaDeSegmentosAsociada, free);
	free(proceso); 
}

t_peticion* recibirPeticionCreacionDeSegmento(int socketCliente) {

	int size, desplazamiento = 0; 

	t_peticion* peticion = malloc(sizeof(t_peticion)); 

	t_segmento* segmentoPedido = malloc(sizeof(t_peticion)); 

	peticion->segmento = segmentoPedido; 

	void* buffer = recibirBuffer(socketCliente, &size); 

	desplazamiento+=sizeof(uint32_t); 

	memcpy(&peticion->pid, buffer + desplazamiento, sizeof(uint32_t));

	desplazamiento+=(2*sizeof(uint32_t)); 

	memcpy(&peticion->segmento->id,buffer+desplazamiento,sizeof(uint32_t)); 

	desplazamiento +=(3*sizeof(uint32_t)); 

	memcpy(&peticion->segmento->direccionBase, buffer+desplazamiento,sizeof(uint32_t)); 

	return peticion; 

}
