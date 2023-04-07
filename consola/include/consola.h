#ifndef CLIENT_H_
#define CLIENT_H_

#include<stdio.h>
#include<stdlib.h>
#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>
#include<readline/readline.h>

#include "utils.h"

/**
 * @brief Inicializa el logger del programa.
 * @return Un puntero al logger creado.
 */
t_log* iniciar_logger(void);

/**
 * @brief Inicializa la configuración del programa.
 * @return Un puntero a la estructura de configuración creada.
 */
t_config* iniciar_config(void);

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

#endif /* CLIENT_H_ */