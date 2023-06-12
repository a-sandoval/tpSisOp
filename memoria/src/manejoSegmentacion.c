#include "include/manejoSegmentacion.h"

t_segmento * segmento0; 

void creacionEspacio(){
    espacioDeUsuario = malloc (config_get_int_value(config,"TAM_MEMORIA")); 
	if (espacioDeUsuario == NULL) error ("No se pudo alocar memoria al espacio de usuario.");
	atexit (liberarEspacioDeUsuario);
	segmento0 = crearSegmentoCompartido (); 
	espacioDeUsuario = segmento0; 
	atexit (liberarSegmentoCompartido);
}

t_segmento* crearSegmentoCompartido() {

	segmento0 = malloc(sizeof(t_segmento)); 
    segmento0->id=0; 
    segmento0->direccionBase=0; 
    segmento0->tamanio = config_get_int_value(config,"TAM_SEGMENTO_0"); 

	return segmento0; 
}

 //Federico esto nunca se libera
void liberarEspacioDeUsuario () {
	free (espacioDeUsuario);
}

void liberarSegmentoCompartido () {
	free (segmento0);
}