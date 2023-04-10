#ifndef CLIENT_H_
#define CLIENT_H_

#include<stdio.h>
#include<stdlib.h>
#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>
#include<readline/readline.h>
#include<readline/history.h>

#include "shared/include/utilsCliente.h"

char *listaComandos[] = {
    "CREATE_SEGMENT", "DELETE_SEGMENT", "I/O", 
    "EXIT", 
    "F_CLOSE", "F_OPEN", "F_READ", "F_SEEK", "F_TRUNCATE", "F_WRITE", 
    "MOV_IN", "MOV_OUT", 
    "SET", "SIGNAL", "WAIT", "YIELD"
};

/**
 * @brief Lee la entrada de la consola y la registra en el logger.
 * @param logger Un puntero al logger utilizado por el programa.
 */
void leer_consola(t_log*);

/**
 * @brief Termina la ejecución del programa y libera la memoria utilizada.
 * @param conexion Socket utilizado en una conexión a ser desconectado.
 * @param logger Un puntero al logger utilizado por el programa para liberarlo.
 * @param config Un puntero a la estructura de configuración utilizada por el programa para liberarla.
 */
void terminar_programa(int , t_log*, t_config*);

char **autoCompletar(const char*, int, int);

char *comandoPosible(const char*, int);

#endif /* CLIENT_H_ */