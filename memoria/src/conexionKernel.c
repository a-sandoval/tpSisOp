#include "memoria/include/conexionKernel.h"

t_peticion* peticion; 
int cantidadMaximaSegmentos; 

int ejecutarServidorKernel(int *socketCliente){
	
	log_debug(logger, "Conectado el Kernel");
	
	while (1) {
		int peticionRealizada = recibirOperacion(*socketCliente);
		cantidadMaximaSegmentos = config_get_int_value(config,"CANT_SEGMENTOS"); 
		
		log_debug(logger,"Se recibio peticion %d del Kernel", peticionRealizada); 

		switch (peticionRealizada) {
			case NEWPCB:
				t_proceso* procesoNuevo = crearProcesoEnMemoria(recibirPID(*socketCliente)); 
				enviarTablaSegmentos(procesoNuevo);
				break;
            case ENDPCB:
				uint32_t pid = recibirPID(*socketCliente);
				liberarTodosLosSegmentos(pid); 
				list_remove_and_destroy_element(tablaDeTablasDeSegmentos,pid,(void*)eliminarProcesoDeMemoria); 
				log_info(logger, "Eliminación de Proceso PID: %d",pid);
				break;
			case CREATE_SEGMENT_OP:
				peticion = recibirPeticionCreacionDeSegmento(*socketCliente); 
				op_code resultado = ubicarSegmentosEnEspaciosLibres(peticion); 
				procesarResultado((int)resultado,*socketCliente); 
				break;
            case DELETE_SEGMENT_OP:
				recibirYProcesarPeticionEliminacionSegmento(*socketCliente); 
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

void procesarResultado(int resultado, int socketKernel) {

	switch(resultado) {
		case SUCCESS: 
		enviarCodOp(SUCCESS,socketKernel); 
		enviarTablaSegmentos((t_proceso*)list_get(tablaDeTablasDeSegmentos,peticion->pid)); 
		break; 

		case OUTOFMEMORY: 
		enviarCodOp(OUTOFMEMORY,socketKernel); 
		break; 

		case COMPACTACION:
		break; 

	}

}

t_list* crearTablaDeSegmentosInicial() {

    t_list* tablaDeSegmentos = list_create(); 
    list_add(tablaDeSegmentos,(void*)segmento0);

	for(int i=1;i<cantidadMaximaSegmentos;i++) {

		t_segmento* segmentoVacio = malloc(sizeof(t_segmento)); 
		segmentoVacio->id = i; 
		segmentoVacio->direccionBase = UINT32_MAX;
		segmentoVacio->tamanio=0; 

		list_add(tablaDeSegmentos,(void*)segmentoVacio); 

	}

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



void eliminarProcesoDeMemoria(t_proceso* proceso) {
	list_remove_element(proceso->tablaDeSegmentosAsociada, (void*)segmento0); 
	list_destroy_and_destroy_elements(proceso->tablaDeSegmentosAsociada, free);
	free(proceso); 
}

void deleteSegment(uint32_t pid, uint32_t segmentId) {

	t_proceso* procesoBuscado = (t_proceso*)list_get(tablaDeTablasDeSegmentos,pid); 

	t_segmento* segmentoAEliminar = (t_segmento*)list_get(procesoBuscado->tablaDeSegmentosAsociada,segmentId);

	log_info(logger, "PID: %d - Eliminar Segmento: %d - Base: %d - TAMAÑO: %d",pid,segmentId,segmentoAEliminar->direccionBase,segmentoAEliminar->tamanio); 

	convertirSegmentoEnHuecoLibre((void*)segmentoAEliminar); 

	segmentoAEliminar->direccionBase=UINT32_MAX; 

	segmentoAEliminar->tamanio=0; 

	list_replace_and_destroy_element(procesoBuscado->tablaDeSegmentosAsociada,segmentId,segmentoAEliminar, free);  

	enviarTablaSegmentos(procesoBuscado); 

	//Revisar una vez implementemos compactación para incluir ese nuevo segm libre
}

void convertirSegmentoEnHuecoLibre(void* segmento) {

	t_segmento* aConvertir = (t_segmento*) segmento; 

	t_hueco_libre* nuevoHuecoLibre = malloc(sizeof(t_hueco_libre)); 
	
	nuevoHuecoLibre->direccionBase = aConvertir->direccionBase; 
	nuevoHuecoLibre->tamanioHueco = aConvertir->tamanio; 

	list_add(huecosLibres, (void*)nuevoHuecoLibre);
}

void liberarTodosLosSegmentos(uint32_t pid) {
	
	t_list* tablaDeSegmentos = (t_list*)list_get(tablaDeTablasDeSegmentos,pid); 

	list_iterate(tablaDeSegmentos,convertirSegmentoEnHuecoLibre); 
}
