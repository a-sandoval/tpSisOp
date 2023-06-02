#include "memoria/include/conexionKernel.h"


int ejecutarServidorKernel(int socket){
    socketCliente = socket; 
	
	log_info(logger, "Entre a servidor kernel");
	while (1) {
		int codOP = recibirOperacion(socketCliente);
		switch (codOP) {
			case MENSAJE:
				log_info(logger, "Se autoriza continuar :)");
				break;
			case PAQUETE:
				break;
			case -1:
				log_info(logger, "soyservidor kernel");
				log_error(logger, "El cliente se desconecto");
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