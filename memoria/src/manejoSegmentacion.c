#include "memoria/include/manejoSegmentacion.h"

t_segmento * segmento0; 
void* espacioDeUsuario; 
t_list* huecosLibres; 
t_list* tablaDeTablasDeSegmentos; 

// Creación estructuras
void creacionEspacio(){
    espacioDeUsuario = malloc (config_get_int_value(config,"TAM_MEMORIA")); 
	if (espacioDeUsuario == NULL) error ("No se pudo alocar memoria al espacio de usuario.");
	atexit (liberarEspacioDeUsuario);
	segmento0 = crearSegmentoCompartido (); 
	espacioDeUsuario = segmento0; 
	atexit (liberarSegmentoCompartido);
}

void creacionListaHuecosLibres() {

    huecosLibres = list_create(); 
    
    t_hueco_libre* primerHuecoLibre = malloc(sizeof(t_hueco_libre)); 

    primerHuecoLibre->direccionBase = config_get_int_value(config,"TAM_SEGMENTO_0"); 
    primerHuecoLibre->tamanioHueco = config_get_int_value(config,"TAM_MEMORIA") - primerHuecoLibre->direccionBase; 

	list_add(huecosLibres,(void*)primerHuecoLibre); 
    
}

t_segmento* crearSegmentoCompartido(){

	segmento0 = malloc(sizeof(t_segmento)); 
    segmento0->id=0; 
    segmento0->direccionBase=0; 
    segmento0->tamanio = config_get_int_value(config,"TAM_SEGMENTO_0"); 

	return segmento0; 
}

void liberarEspacioDeUsuario() {
	free (espacioDeUsuario);
}

void liberarSegmentoCompartido() {
	free (segmento0);
}

// Esquema de memoria

void ubicarSegmentosEnEspaciosLibres(t_peticion* peticion){
	
    char* algoritmoAUtilizar = config_get_string_value(config, "ALGORITMO_ASIGNACION");

	if (!strcmp(algoritmoAUtilizar, "FIRST"))
    {
        ubicarSegmentosPorFirst(peticion);
    }
    else if (!strcmp(algoritmoAUtilizar, "BEST"))
    {
        ubicarSegmentosPorBest(peticion);
    }
    else if (!strcmp(algoritmoAUtilizar, "WORST"))
    {
        ubicarSegmentosPorWorst(peticion); 
    }

    log_error(loggerError, "ERROR EN ALGORITMO DE ASIGNACION");
    abort();
}

//Implementacion First
void ubicarSegmentosPorFirst(t_peticion* peticion){
	
    uint32_t tamanioSegmento = peticion->segmento->tamanio; 

    t_hueco_libre* huecoLibre; 

    for (int i=0;i<list_size(huecosLibres);i++) {
        
        huecoLibre = ((t_hueco_libre*)list_get(huecosLibres,i)); 
      
        if(huecoLibre->tamanioHueco >= tamanioSegmento) {
			
            peticion->segmento->direccionBase = huecoLibre->direccionBase;
            
            log_debug(logger, "Se ha encontrado un espacio para el segmento");
            loggearCreacionDeSegmento(peticion); 
            asignacionAlEspacioDeMemoria(peticion->segmento);
            agregarSegmentoATablaDeSegmentosPCB(peticion); 
			reducirHuecosLibres(peticion->segmento, i);
            return; 
        }
    }
    corroborarPosibilidadDeCompactacion();
}


void loggearCreacionDeSegmento(t_peticion* peticion) {
    log_info(logger,"PID: %d - Crear Segmento: %d - Base: %d - TAMAÑO: %d",peticion->pid,peticion->segmento->id,peticion->segmento->direccionBase,peticion->segmento->tamanio); 
}

void asignacionAlEspacioDeMemoria(t_segmento* segmento){
    t_segmento* offsetPointer = (t_segmento*)((char*)espacioDeUsuario + segmento->direccionBase);
	*offsetPointer = *segmento;
}

void agregarSegmentoATablaDeSegmentosPCB(t_peticion* peticion){
	int pidProceso = peticion -> pid;
	t_segmento* segmentoAAgregar = peticion->segmento;
	t_proceso* proceso = (t_proceso*) list_get(tablaDeTablasDeSegmentos,pidProceso);
	
	list_add(proceso->tablaDeSegmentosAsociada, (void*) segmentoAAgregar);
}

void reducirHuecosLibres(t_segmento* segmento, int indiceHueco) {
    t_hueco_libre* aModificar = list_get(huecosLibres,indiceHueco); 
    aModificar->tamanioHueco -= segmento->tamanio; 
    if(aModificar->tamanioHueco == 0) {
    	list_remove_and_destroy_element(huecosLibres,indiceHueco,free); 
    }
    else {
        aModificar->direccionBase = segmento->tamanio; 
    }
}

//Implementacion Worst
void ubicarSegmentosPorWorst(t_peticion* peticion){
	int sobraDeHueco = 0;
    uint32_t tamanioSegmento = peticion->segmento->tamanio; 

    t_hueco_libre* huecoLibre; 

    for (int i=0;i<list_size(huecosLibres);i++) {
        
        huecoLibre = ((t_hueco_libre*)list_get(huecosLibres,i)); 
		/*
		maximo
		si es 0 no entra en ninguno
		*/
      
        if(huecoLibre->tamanioHueco >= tamanioSegmento && (quedaLibreAsignandoSegmentoAHueco(tamanioSegmento, huecoLibre) > sobraDeHueco)) {
			
			t_hueco_libre* huecoProvisorio=huecoLibre
			sobraHueco = quedaLibreAsignandoSegmentoAHueco(tamanioSegmento, huecoLibre);

            peticion->segmento->direccionBase = huecoLibre->direccionBase;
            log_debug(logger, "Se ha encontrado un espacio para el segmento");
            loggearCreacionDeSegmento(peticion); 
            asignacionAlEspacioDeMemoria(peticion->segmento);
            agregarSegmentoATablaDeSegmentosPCB(peticion); 
			reducirHuecosLibres(peticion->segmento, i);
            return; 
        }
    }
    corroborarPosibilidadDeCompactacion();
}




















// Cositas mockeadas que linda palabra mockear
void corroborarPosibilidadDeCompactacion() {}

void ubicarSegmentosPorBest(t_peticion* peticion){}
void ubicarSegmentosPorWorst(t_peticion* peticion){} 



