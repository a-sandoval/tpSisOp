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

void ubicarSegmentosPorFirst(t_peticion* peticion){
	
    uint32_t tamanioSegmento = peticion->segmento->tamanio; 

    t_hueco_libre* huecoLibre; 

    for (int i=0;i<list_size(huecosLibres);i++) {
        
        huecoLibre = ((t_hueco_libre*)list_get(huecosLibres,i)); 
      
        if(huecoLibre->tamanioHueco >= tamanioSegmento) {
			
            peticion->segmento->direccionBase = huecoLibre->direccionBase;
            log_debug(logger, "Se ha encontrado un espacio para el segmento");

            asignacionAlEspacioDeMemoria(peticion);
            actualizarTablaDeSegmentosProceso(peticion); 
			actualizarListaDeHuecosLibres(peticion);
            return; 
        }
    }
}

void asignacionAlEspacioDeMemoria(t_peticion* peticion){

    int blockSize = 1024; // Tamaño del bloque de memoria en bytes
    void* memoryBlock = malloc(blockSize); // Puntero al bloque de memoria asignado

    // Ejemplo de uso: escribir y leer valores en el bloque de memoria
    int intValue = 42;
    int* offsetPointer = (int*)((char*)memoryBlock + 4); // Desplazarse 4 bytes dentro del bloque de memoria
    *offsetPointer = intValue;

    // Leer el valor desde el bloque de memoria
    int readValue = *(int*)((char*)memoryBlock + 4);
    printf("Valor leído: %d\n", readValue);

    // Liberar el bloque de memoria
    free(memoryBlock);

    return 0
	
}
