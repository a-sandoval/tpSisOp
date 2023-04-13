#ifndef LISTAS_H
#define LISTAS_h
#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netdb.h>
#include<string.h>
#include<commons/log.h>
#include <commons/config.h>
#endif 


typedef struct NodoIdentificador{
    char* info;
    struct NodoIdentificador* siguiente;
    
} Nodo;

typedef struct Lista {
    Nodo* cabeza;
} Lista;

void insertar(Lista*, char*); 
int tamanio_lista(Lista*); 