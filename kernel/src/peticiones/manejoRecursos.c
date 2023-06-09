#include "kernel/include/peticiones/manejoRecursos.h"

//int *instanciasRecursos;
//t_list *recursos;
//char **nombresRecursos;
int cantidadRecursos;

// crear colas de bloqueo
void crearColasBloqueo()
{
	instanciasRecursos = NULL; 
	recursos = list_create();
    nombresRecursos = obtenerRecursos();
    char **cantInstanciasRecursos = obtenerInstanciasRecursos();
    int tamanio = tamanioArrayCharDoble(cantInstanciasRecursos);

    cantidadRecursos=tamanio;
    for (int i = 0; i < tamanio; i++)
    {

        int instanciasConvertEntero = atoi(cantInstanciasRecursos[i]);
        instanciasRecursos=realloc(instanciasRecursos,(i+1)*sizeof(int));
        instanciasRecursos[i]=instanciasConvertEntero;

        t_list *colaBloqueo = list_create();
        list_add(recursos, (void *)colaBloqueo);
    }

    destruirArrayCharDoble(cantInstanciasRecursos);
}

// Devuelve el indice que se corresponde al recurso correspondiente, -1 si no lo encuentra
int indiceRecurso (char * recurso){
    int tamanio = tamanioArrayCharDoble(nombresRecursos);

    for (int i = 0; i < tamanio; i++)
        if (!strcmp(recurso, nombresRecursos[i]))
            return i;
    return -1;
}

void liberarColasBloqueo(){
	destruirInstanciasRecursos();
	destruirArrayCharDoble(nombresRecursos);
	destruirRecursos();
}

//Destruccion de colas de bloqueo
void destruirInstanciasRecursos(){
	free(instanciasRecursos);
}


void destruirArrayCharDoble (char ** array){
	int tamanio=tamanioArrayCharDoble(array);
	for (int i = 0; i<tamanio; i++) free(array[i]);
	free(array);
}


void closureMatriz(void* colaBloqueados){

	t_list* cola = (t_list*)colaBloqueados;

	list_iterate(cola, closurePCB);

	//putchar('\n');

}

void closurePCB(void* pcbActual){

	t_pcb* pcb = (t_pcb*) pcbActual;
}


void colaBloqueadosDestroyer(void* colaBloqueados){
	list_destroy_and_destroy_elements(colaBloqueados,(void*)destruirPCB);
}

void destruirRecursos(){
	list_destroy_and_destroy_elements(recursos, colaBloqueadosDestroyer);
}

//Calcula el tamanio de un array char doble que vienen de los config y termina en NULL
int tamanioArrayCharDoble (char **arreglo){
	    /*
		int tamano = 0;
	    while (arreglo[tamano] != NULL) 
	        tamano++;
	    */
	    return string_array_size(arreglo);

}

