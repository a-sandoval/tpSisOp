#ifndef LINEACOMANDO_H_
#define LINEACOMANDO_H_

#include<stdio.h>
#include<stdlib.h>
#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>
#include<readline/readline.h>
#include<readline/history.h>

/**
 * @brief Lee la entrada de la consola y la registra en el logger.
 * @param logger Un puntero al logger utilizado por el programa.
 */
void leer_consola(t_log* logger, int conexion);

char **autoCompletar(const char*, int, int);

char *comandoPosible(const char*, int);

#endif /* LINEACOMANDO_H_ */