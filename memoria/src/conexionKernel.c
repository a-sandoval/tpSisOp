#include "memoria/include/conexionKernel.h"

t_peticion *peticion;
int cantidadMaximaSegmentos;
uint32_t direccionBaseSegmento;

int ejecutarServidorKernel(int *socketCliente){

	while (1)
	{
		int peticionRealizada = recibirOperacion(*socketCliente);
		cantidadMaximaSegmentos = config_get_int_value(config, "CANT_SEGMENTOS");

		switch (peticionRealizada)
		{
		case NEWPCB:
			t_proceso *procesoNuevo = crearProcesoEnMemoria(recibirPID(*socketCliente));
			enviarTablaSegmentos(procesoNuevo);
			break;
		case ENDPCB:
			uint32_t pid = recibirPID(*socketCliente);
			liberarTodosLosSegmentos(pid);
			eliminarProcesoDeMemoria (buscarProcesoSegun (pid));
			log_info(logger, "Eliminación de Proceso PID: %d", pid);
			break;
		case CREATE_SEGMENT_OP:
			peticion = recibirPeticionCreacionDeSegmento(*socketCliente);
			op_code resultado = ubicarSegmentosEnEspaciosLibres(peticion);
			procesarResultado((int)resultado, *socketCliente);
			free (peticion);
			break;
		case DELETE_SEGMENT_OP:
			recibirYProcesarPeticionEliminacionSegmento(*socketCliente);
			break;
		case -1:
			log_error(logger, "El Kernel se desconecto");
			return EXIT_FAILURE;
			break;
		default:
			log_warning(logger, "Operacion desconocida. No quieras meter la pata");
			break;
		}
	}
}

void procesarResultado(int resultado, int socketKernel){

	switch (resultado){
	case SUCCESS:
		enviarCodOp(SUCCESS, socketKernel);
		enviarTablaSegmentos(buscarProcesoSegun(peticion->pid));
		break;

	case OUTOFMEMORY:
		enviarCodOp(OUTOFMEMORY, socketKernel);
		break;

	case COMPACTACION:
		enviarCodOp(COMPACTACION,socketKernel); 
		recibirMensaje(socketKernel); 
		compactar();
		break;
	}
}

t_list *crearTablaDeSegmentosInicial(){

	t_list *tablaDeSegmentos = list_create();
	list_add(tablaDeSegmentos, (void *)segmento0);

	for (int i = 1; i < cantidadMaximaSegmentos; i++)
	{

		t_segmento *segmentoVacio = malloc(sizeof(t_segmento));
		segmentoVacio->id = i;
		segmentoVacio->direccionBase = UINT32_MAX;
		segmentoVacio->tamanio = 0;

		list_add(tablaDeSegmentos, (void *)segmentoVacio);
	}
	return tablaDeSegmentos;
}

t_proceso *crearProcesoEnMemoria(uint32_t pid){

	t_proceso *procesoNuevo = malloc(sizeof(t_proceso));
	procesoNuevo->pid = pid;
	procesoNuevo->tablaDeSegmentosAsociada = crearTablaDeSegmentosInicial();
	list_add(tablaDeTablasDeSegmentos, (void *)procesoNuevo);
	log_info(logger, "Creacion de Proceso PID: %d", procesoNuevo->pid);

	return procesoNuevo;
}

void eliminarProcesoDeMemoria(t_proceso *proceso){
	list_remove_element (tablaDeTablasDeSegmentos, (void *) proceso);
	list_destroy_and_destroy_elements(proceso->tablaDeSegmentosAsociada, free);
	free(proceso);
}

void deleteSegment(uint32_t pid, uint32_t segmentId){

	t_proceso *procesoBuscado = buscarProcesoSegun(pid);

	t_segmento *segmentoAEliminar = (t_segmento *)list_get(procesoBuscado->tablaDeSegmentosAsociada, segmentId);

	log_info(logger, "PID: %d - Eliminar Segmento: %d - Base: %d - TAMAÑO: %d", pid, segmentId, segmentoAEliminar->direccionBase, segmentoAEliminar->tamanio);

	convertirSegmentoEnHuecoLibre((void *)segmentoAEliminar);

	segmentoAEliminar->direccionBase = UINT32_MAX;

	segmentoAEliminar->tamanio = 0;

	//list_replace(procesoBuscado->tablaDeSegmentosAsociada, segmentId, segmentoAEliminar);

	enviarTablaSegmentos(procesoBuscado);

	// Revisar una vez implementemos compactación para incluir ese nuevo segm libre
}

bool direccionMasBaja (void * huecoLibreUno, void * huecoLibreDos) {
	t_hueco_libre * huecoUno = (t_hueco_libre * ) huecoLibreUno;
	t_hueco_libre * huecoDos = (t_hueco_libre * ) huecoLibreDos;
	return (huecoUno->direccionBase < huecoDos->direccionBase); 
}

void convertirSegmentoEnHuecoLibre(void *segmento){

	t_segmento *aConvertir = (t_segmento *)segmento;
	if (aConvertir->direccionBase != UINT32_MAX && aConvertir->tamanio != 0)
	{
		direccionBaseSegmento = aConvertir->direccionBase;

		t_hueco_libre *nuevoHuecoLibre = (t_hueco_libre*)list_find(huecosLibres, contieneDireccionBaseEnSuTamanio);

		if (nuevoHuecoLibre != NULL)
			nuevoHuecoLibre->tamanioHueco = maximoEntre(aConvertir->tamanio, nuevoHuecoLibre->tamanioHueco);

		else
		{
			nuevoHuecoLibre = malloc(sizeof(t_hueco_libre));
			nuevoHuecoLibre->direccionBase = aConvertir->direccionBase;
			nuevoHuecoLibre->tamanioHueco = aConvertir->tamanio;

			list_add_sorted(huecosLibres, (void *)nuevoHuecoLibre, direccionMasBaja);
		}
	}
	else
		return;
}

void listarHuecosLibres () {
	t_hueco_libre * temp;
	for (int i = 0; i < list_size (huecosLibres); i++) {
		temp = list_get (huecosLibres, i);
		log_debug (logger, "Encontrado un hueco libre con direccion %d y tamaño %d", temp->direccionBase, temp->tamanioHueco);
	}
}

bool contieneDireccionBaseEnSuTamanio(void *huecoLibre){	
	t_hueco_libre* unHuecoLibre = (t_hueco_libre*) huecoLibre; 
	
	return unHuecoLibre->direccionBase <= direccionBaseSegmento && (unHuecoLibre->direccionBase + unHuecoLibre->tamanioHueco) >= direccionBaseSegmento;
}

uint32_t maximoEntre(uint32_t unTamanio, uint32_t otroTamanio){

	return unTamanio >= otroTamanio ? unTamanio : otroTamanio;
}

void liberarTodosLosSegmentos(uint32_t pid){

	t_list *tablaDeSegmentos = (buscarProcesoSegun(pid))->tablaDeSegmentosAsociada;
	if (tablaDeSegmentos != NULL)
	{
		list_remove_element(tablaDeSegmentos,(void*)segmento0); 
		list_iterate(tablaDeSegmentos, convertirSegmentoEnHuecoLibre);
	}
}

