#include "memoria/include/algoritmosAsignacion.h"

t_list* tablaDeTablasDeSegmentos; 

// Esquema de memoria

op_code ubicarSegmentosEnEspaciosLibres(t_peticion* peticion){
	
    char* algoritmoAUtilizar = config_get_string_value(config, "ALGORITMO_ASIGNACION");
    op_code resultado; 
	if (!strcmp(algoritmoAUtilizar, "FIRST"))
    {
        resultado = ubicarSegmentosPorFirst(peticion);
        return resultado; 
    }
    else if (!strcmp(algoritmoAUtilizar, "BEST"))
    {
        resultado = ubicarSegmentosPorBest(peticion);
        return resultado; 
        
    }
    else if (!strcmp(algoritmoAUtilizar, "WORST"))
    {
        resultado = ubicarSegmentosPorWorst(peticion); 
        return resultado; 
        
    }

    error("ERROR EN ALGORITMO DE ASIGNACION");
    return 0;
}

//Implementacion First
op_code ubicarSegmentosPorFirst(t_peticion* peticion){
	
    uint32_t tamanioSegmento = peticion->segmento->tamanio; 

    t_hueco_libre* huecoLibre; 

    for (int i=0;i<list_size(huecosLibres);i++) {
        
        huecoLibre = ((t_hueco_libre*)list_get(huecosLibres,i)); 
      
        if(huecoLibre->tamanioHueco >= tamanioSegmento) {
			
            peticion->segmento->direccionBase = huecoLibre->direccionBase;
            
            log_debug(logger, "Se ha encontrado un espacio para el segmento");
            loggearCreacionDeSegmento(peticion); 
            agregarSegmentoATablaDeSegmentosPCB(peticion); 
			reducirHuecosLibres(peticion->segmento, i);
            return SUCCESS; 
        }
    }
    corroborarPosibilidadDeCompactacion();

    return OUTOFMEMORY; 
}


void loggearCreacionDeSegmento(t_peticion* peticion) {
    log_info(logger,"PID: %d - Crear Segmento: %d - Base: %d - TAMAÑO: %d",peticion->pid,peticion->segmento->id,peticion->segmento->direccionBase,peticion->segmento->tamanio); 
}



void agregarSegmentoATablaDeSegmentosPCB(t_peticion* peticion){
	int pidProceso = peticion -> pid;
	t_segmento* segmentoAAgregar = peticion->segmento;
	t_proceso* proceso = (t_proceso*) list_get(tablaDeTablasDeSegmentos,pidProceso);
	
	list_replace_and_destroy_element(proceso->tablaDeSegmentosAsociada,peticion->segmento->id,(void*)segmentoAAgregar,free); 

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

//Implementacion Best
op_code ubicarSegmentosPorBest(t_peticion* peticion){
    uint32_t tamanioSegmento = peticion->segmento->tamanio; 
    t_hueco_libre* huecoLibre = NULL; 
    t_hueco_libre* huecoAAsignar = NULL;
    int indiceHueco;
    bool encontrado = false; 
    int32_t tamanioHuecoMenor = -(__INT32_MAX__);

    for (int i=0;i<list_size(huecosLibres);i++) {
        huecoLibre = ((t_hueco_libre*)list_get(huecosLibres,i));

        if((huecoLibre->tamanioHueco >= tamanioSegmento) && ((int32_t)huecoLibre->tamanioHueco < tamanioHuecoMenor)){
            huecoAAsignar = huecoLibre;
            tamanioHuecoMenor = huecoAAsignar->tamanioHueco;
            indiceHueco = i;
            encontrado=true; 
            break;
        }
    }
    if (encontrado) {

        peticion->segmento->direccionBase = huecoAAsignar->direccionBase;
        log_debug(logger, "Se ha encontrado un espacio para el segmento");
        loggearCreacionDeSegmento(peticion); 
        agregarSegmentoATablaDeSegmentosPCB(peticion); 
	    reducirHuecosLibres(peticion->segmento, indiceHueco);
        return SUCCESS;

    }
   
    else {
        corroborarPosibilidadDeCompactacion(); 
        return OUTOFMEMORY;
    }
    
}

//Implementacion Worst
op_code ubicarSegmentosPorWorst(t_peticion* peticion){

    uint32_t tamanioSegmento = peticion->segmento->tamanio; 
    t_hueco_libre* huecoLibre; 
    t_hueco_libre* huecoAAsignar;
    int indiceHueco;
    uint32_t tamanioHuecoMayor = 0;

    for (int i=0;i<list_size(huecosLibres);i++) {
        huecoLibre = ((t_hueco_libre*)list_get(huecosLibres,i));

        if(huecoLibre->tamanioHueco > tamanioHuecoMayor){
            huecoAAsignar = huecoLibre;
            tamanioHuecoMayor = huecoAAsignar->tamanioHueco;
            indiceHueco = i;
        }
    }

    if(huecoAAsignar->tamanioHueco >= tamanioSegmento) {
        peticion->segmento->direccionBase = huecoAAsignar->direccionBase;
        log_debug(logger, "Se ha encontrado un espacio para el segmento");
        loggearCreacionDeSegmento(peticion); 

        agregarSegmentoATablaDeSegmentosPCB(peticion); 
		reducirHuecosLibres(peticion->segmento, indiceHueco);
        return SUCCESS; 
    }
    else
    {
        corroborarPosibilidadDeCompactacion(); 
        log_error(logger, "No se ha encontrado lugar para el segmento");
        return OUTOFMEMORY; 
        //proximamente será una petición de compactación
    }
}


// Cositas mockeadas que linda palabra mockear
void corroborarPosibilidadDeCompactacion() {}