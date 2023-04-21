#ifndef LISTAS_H
#define LISTAS_H

#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netdb.h>
#include<string.h>
#include<commons/log.h>
#include<commons/config.h>

typedef struct NodoIdentificador{
    char* info;
    struct NodoIdentificador* siguiente;

} Nodo;

typedef struct Lista {
    Nodo* cabeza;
} Lista;

//bool esClaveValida(Lista*, char*);
void insertar(Lista*, char*); 
void borrarLista(Lista*);
void imprimirLista(Lista* lista);

//int tamanio_lista(Lista* lista);

#endif 