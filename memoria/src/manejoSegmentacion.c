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

void ubicarSegmentosEnEspaciosLibres(t_segmento* segmento){

}
