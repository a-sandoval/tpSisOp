#include "../include/manejoListas.h" 

/*void insertar(Lista* lista, char* clave) {

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
}*/

//Version:Brisa
void insertar(Lista* lista, char* clave) {

    Nodo *nodo, *r, *ant;
    nodo = (Nodo*)malloc(sizeof(Nodo));
    nodo->info = malloc(sizeof(char)*(strlen(clave)+1));
    strcpy(nodo->info,clave);
    
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
void borrarLista(Lista* lista){
    Nodo* ant,*act,*sig;
    ant = lista->cabeza;
    
    while(ant != NULL && act!=NULL && sig!=NULL){
        act=ant->siguiente;
        sig=act->siguiente;
        free(ant->info);
        free(act->info);
        free(ant);
        free(act);
        ant=sig;
    }

    //free(lista->cabeza); 
    free(lista); 
}*/

/*ORIGINAL 
*/
void borrarLista(Lista* lista){
    Nodo* aux; 
    Nodo* anterior; 
    aux = lista->cabeza;
    anterior=aux; 
    
    while(aux!=NULL) {

        aux = aux->siguiente; 
        free(anterior->info); 
        free(anterior); 
        anterior=aux; 

     }
    free(lista); 
}



bool esClaveValida(Lista* lista, char* clave){
    Nodo *r;
    r = lista->cabeza;

    while(r != NULL){
        if(!strcmp(r->info, clave)){
            return true;
        }
        r = r->siguiente;
    }   
    return false; 
}

void imprimirLista(Lista* lista){

    Nodo *r;

    r = lista->cabeza; 

    while(r != NULL){
        printf("Valor: %s",r->info);
        r = r->siguiente;
    }
}

