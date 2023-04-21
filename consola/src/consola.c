#include "consola/include/consola.h"

int main(int, char *archivos[]) {

    // inicializacion de las variables

    int conexion;
    char *ip, *puerto;
    logger = iniciarLogger("consola.log", "consola");

    config = iniciarConfiguracion(archivos[2], logger);
    ip = confGet(config, "IP_KERNEL");
    puerto = confGet(config, "PUERTO_KERNEL");

    // conexion al kernel

    conexion = crear_conexion(ip, puerto);

    if (!(conexion + 1)) { 
        log_error(logger, "No se pudo conectar a la Kernel por la ip %s y puerto %s", ip, puerto);
        log_destroy(logger);
        config_destroy(config);
        return 1;
    }
    
    // apertura del archivo de pseudocodigo

    FILE *codigo = fopen(archivos[1], "r");

    if (codigo == NULL) {
        log_error(logger, "No se pudo abrir el archivo %s", archivos[1]);
        log_destroy(logger);
        config_destroy(config);
        close(conexion);
        return 1;
    }
    //rl_attempted_completion_function = autoCompletar;
    //leer_consola(logger, conexion);

    //string_array_new y string_replace dan leaks

    // leer el archivo y enviar paquetes del codigo
    enviar_mensaje(archivos[3], conexion);
    char lineaCodigo[1024];
    while(!feof(codigo)) {
        fgets(lineaCodigo, sizeof(lineaCodigo), codigo);
        char **listaParametros = string_n_split(lineaCodigo, 4, " ");
        t_comando comando = buscarComando(listaParametros[0]);

        if (comando.nombre != NULL) {
            char *temp = string_array_pop(listaParametros);
            string_array_push(&listaParametros, string_substring_until(temp, strlen(temp) - 1));
            free(temp);
            
            t_comando_total comComp = prepararComando(comando, listaParametros);

            t_paquete *paquete = crear_paquete();
            //agregar_a_paquete(paquete, &comComp, sizeof(comComp) + (sizeof(char) * (comComp.longNombre + comComp.longParametros[0] + comComp.longParametros[1] + comComp.longParametros[2])));
            /*agregar_a_paquete(paquete, (void)comComp.cantParametros, sizeof(int));
            agregar_a_paquete(paquete, (void)comComp.longNombre, sizeof(int));
            agregar_a_paquete(paquete, (void)comComp.longParametros[0], sizeof(int));
            agregar_a_paquete(paquete, (void)comComp.longParametros[1], sizeof(int));
            agregar_a_paquete(paquete, (void)comComp.longParametros[2], sizeof(int)); */
            agregar_a_paquete(paquete, comComp.nombre, sizeof(char) * comComp.longNombre + 1);
            int i = 0;
            while(!queue_is_empty(comComp.filaParametros)) {
                agregar_a_paquete(paquete, queue_pop(comComp.filaParametros), sizeof(char) * comComp.longParametros[i] + 1);
                i++;
            }

            enviar_paquete(paquete, conexion);
            eliminar_paquete(paquete);
            /*
            printf("%s ", comComp.nombre);
            while(!queue_is_empty(comComp.filaParametros)) printf("%s ", (char *)queue_pop(comComp.filaParametros));
            printf("%d %d %d %d %d %lu", comComp.longNombre, comComp.cantParametros, comComp.longParametros[0], comComp.longParametros[1], comComp.longParametros[2], sizeof(comComp));
            printf("\n");
            */
            free(comComp.nombre);
            queue_destroy(comComp.filaParametros);
        }
        string_array_destroy(listaParametros);
    }

    // cerrar archivos

    fclose(codigo);
    log_destroy(logger);
    config_destroy(config);

    return 0;
}

t_comando buscarComando(char *comando) {
    int i = 0;
    while(listaComandos->cantParametros != -1 && !string_contains(comando, listaComandos[i].nombre)) i++;
    return listaComandos[i];
}

t_comando_total prepararComando(t_comando comando, char **parametros) {
    /*
    printf("%s ", comando.nombre);
    for (int i = 1; i <= comando.cantParametros; i++) 
        printf("\"%s\" ", parametros[i]);
    printf("\n");   
    */
    t_comando_total comandoCompleto;
    comandoCompleto.cantParametros = comando.cantParametros;
    comandoCompleto.filaParametros = queue_create();
    for (int i = 1; i < 4; i++) comandoCompleto.longParametros[i - 1] = (i <= comando.cantParametros) ? strlen(parametros[i]) : 0;
    for (int i = 1; i <= comando.cantParametros; i++) queue_push(comandoCompleto.filaParametros, (void *)parametros[i]);
    comandoCompleto.longNombre = strlen(comando.nombre);
    comandoCompleto.nombre = strdup(comando.nombre);

    return comandoCompleto;
}
