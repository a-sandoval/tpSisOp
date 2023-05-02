#ifndef GLOBAL_H_
#define GLOBAL_H_

    /**
     * @addtogroup Shared
     * @{
     * 
     * @file global.h
     * @authors To-The-End
     * 
     * @brief Archivo Global, utilizado por todos los modulos, con variables y estructuras generales.
     * 
     */

    #include <commons/log.h>
    #include <commons/config.h>
    #include <signal.h>

    /**
     * @ingroup variablesGlobales
     * @{
     * 
     */

    /**
     * @var logger 
     * Logger global para documentar todo evento que ocurra.
     */
 
    /**
     * @var config 
     * Archivo de configuracion utilizado para sacar informacion variada para su uso.
     */
   

    /**
     * 
     * @}
     * Fin de variables globales.
     * 
     * @ingroup enumsGlobales
     * @{
     * 
     */

    /**
     * @enum t_comando
     * @brief Lista de comandos enumerados.
     *
     */
    typedef enum {

    SET,
    MOV_IN,
    MOV_OUT, 
    IO,
    F_OPEN,
    F_CLOSE, 
    F_SEEK,
    F_READ,
    F_WRITE, 
    F_TRUNCATE,
    WAIT,
    SIGNAL,
    CREATE_SEGMENT,
    DELETE_SEGMENT,
    YIELD,
    EXIT, 

    } t_comando; 
    /**
     * 
     * @}
     * Fin de enums globales.
     * 
     * @ingroup structsGlobales
     * @{
     * 
     */

    /**
     * @struct t_instruccion
     * @brief Estructura que sostiene las instrucciones para enviar del Kernel a la CPU (O para que la CPU Parsee la informacion recibida.)
     */
    typedef struct {

        t_comando comando; 
        char* parametros[3]; 

    }t_instruccion; 

    /**
     * 
     * @}
     * Fin de estructuras globales.
     * 
     * @}
     * Fin de global.h
     * 
     */

#endif
