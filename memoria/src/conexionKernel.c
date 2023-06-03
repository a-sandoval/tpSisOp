#include "memoria/include/conexionKernel.h"


int ejecutarServidorKernel(int *socketCliente){
	
	log_info(logger, "Conectado el Kernel");
	while (1) {
		int peticion = recibirOperacion(*socketCliente);
		
		log_info(logger,"Se recibio peticion %d del Kernel", peticion); 

		switch (peticion) {
			case NEWPCB:
				log_info(logger, "Creo tabla de Segmentos y envio segmento 0");
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

    t_segmento *segmento0 = malloc(sizeof(t_segmento)); 
    segmento0->id=0; 
    segmento0->direccionBase=0; 
    segmento0->tamanio = config_get_int_value(config,"TAM_SEGMENTO_0"); 

    t_list* tablaDeSegmentos = list_create(); 
    list_add(tablaDeSegmentos,(void*)segmento0);

    return tablaDeSegmentos;  

}

//serializar tabla de Segmentos

/*
void enviarTablaDeSegmentos(t_list* tablaDeSegmentos){ 
    t_paquete* paquete = crearPaquete();
    
    paquete->codigo_operacion = TABLADESEGMENTOS;

    uint32_t tablaDeSegmentosSize = obtenerTablaDeSegmentos(tablaDeSegmentos);
   
    uint32_t i;
    for(i=0;i<contextoEjecucion->instruccionesSize;i++){
        agregarAPaquete (paquete, list_get(tablaDeSegmentos, i), sizeof(char) * (strlen(list_get(tablaDeSegmentos, i)) + 1 ));
    }


    enviarPaquete(paquete, sockets[0]);

	eliminarPaquete(paquete);
}

uint32_t obtenerTablaDeSegmentosSize(t_list* tablaDeSegmentos){

}

*/