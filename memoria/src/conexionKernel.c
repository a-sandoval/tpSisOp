#include "memoria/include/conexionKernel.h"


int ejecutarServidorKernel(int *socketCliente){
	
	log_info(logger, "Conectado el Kernel");
	
	while (1) {
		int peticion = recibirOperacion(*socketCliente);
		
		log_info(logger,"Se recibio peticion %d del Kernel", peticion); 

		switch (peticion) {
			case NEWPCB:
				log_info(logger, "Creo tabla de Segmentos y envio segmento 0");
				t_list* tablaSegmentos = crearTablaDeSegmentosInicial();
				enviarTablaSegmentos(tablaSegmentos);
				break;
            case ENDPCB:
				log_info(logger, "Elimino estructuras asociadas a este PCB");
				break;
			case CREATE_SEGMENT_OP:
                log_info(logger, "Creo nuevo segmento de memoria");
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

//serializar tabla de Segmentos

void enviarTablaSegmentos(t_list* tablaDeSegmentos){ 
    t_paquete* paquete = crearPaquete();
    
    paquete->codigo_operacion = TABLADESEGMENTOS;

    uint32_t tablaDeSegmentosSize = list_size(tablaDeSegmentos);
   
    uint32_t i;
    for(i=0;i<tablaDeSegmentosSize;i++){
        agregarSegmentoAPaquete(paquete,list_get(tablaDeSegmentos, i));
    }

    enviarPaquete(paquete, sockets[0]);

	eliminarPaquete(paquete);
}

void agregarSegmentoAPaquete(t_paquete* paquete, t_segmento* segmento){
	agregarAPaquete(paquete, (void*)&segmento->id, sizeof(uint32_t));
	agregarAPaquete(paquete, (void*)&segmento->direccionBase, sizeof(uint32_t));
	agregarAPaquete(paquete, (void*)&segmento->tamanio, sizeof(uint32_t));
}

