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

/*typedef struct NodoIdentificador{
    char* info;
    struct NodoIdentificador* siguiente;

} Nodo;

typedef struct Lista {
    Nodo* cabeza;
} Lista;*/

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

    //free(lista->cabeza); //Me parece que ya la libera
    free(lista); //Lo agregue yo: Brisa  
}

/* ORIGINAL
void borrarLista(Lista* lista){
    Nodo* r,*aux;
    r = lista->cabeza;
    
    while(r != NULL){
        free(r->info);
        r=r->siguiente; 
    }

    //free(lista->cabeza); //Me parece que ya la libera
    free(lista); //Lo agregue yo: Brisa  
}
*/


/*bool esClaveValida(Lista* lista, char* clave){
    Nodo *r;
    r = lista->cabeza;
    while(r != NULL && !strcmp(r->info, clave)){//ANtes estaba un not !strcmp
        r = r->siguiente;
    }   
    if (strcmp(r->info,clave)) {
        return true; 
    }
    return false; 
}*/

/* VERSION BRISA, PORQUE NO ME FUNCIONA LA ORIGINAL*/
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

