#include "../include/manejoListas.h" 

void insertar(Lista* lista, char* clave) {

    Nodo *nodo, *r, *ant;
    nodo = (Nodo*)malloc(sizeof(Nodo));
    nodo->info= clave;
    
    r = lista->cabeza; 
    while(r != NULL){
        ant = r;
        r = r->siguiente;
    }
    nodo->siguiente = r;
    if(r != lista->cabeza)
        ant->siguiente = nodo;
    else
        lista->cabeza = nodo; 
}

/*
int tamanio_lista(Lista* lista){
    Nodo *r;
    r = lista->cabeza;
    int tamanio=0; 
    
    while(r != NULL){
        tamanio++;   
        r = r->siguiente;
    }   
    return tamanio;
}
*/

void borrarLista(Lista* lista){
    Nodo* r;
    r = lista->cabeza;
    
    while(r != NULL){
        free(r); 
    }   
}


bool esClaveValida(Lista* lista, char* clave){
    Nodo *r;
    r = lista->cabeza;
    while(r != NULL && !strcmp(r->info, clave)){
        r = r->siguiente;
    }   
    if (strcmp(r->info,clave)) {
        return true; 
    }
    return false; 
}
