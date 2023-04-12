#include "consola/include/lineaComandos.h"

void leer_consola(t_log *logger, int conexion) {
    while (0) {
        char *comando = readline("Usuario@TUKI $ ");
        string_trim(&comando);
        if (!strcmp(comando, "EXIT")) {free(comando); break;}
        if (*comando) {
            enviar_mensaje(comando, conexion);
            add_history(comando); 
        }
        log_info(logger, comando);
        free(comando);
    }
}

char **autoCompletar(const char *texto, int inicio, int) {
    char **lista = NULL;
    if (!inicio) {
        lista = rl_completion_matches(texto, comandoPosible);
    }
    return lista;
}

char *comandoPosible(const char *texto, int estado) {
    int i, len = 0;
    char *nombre;
    if (!estado) {
        i = 0;
        len = strlen(texto);
    }
    //while ((nombre = listaComandos[i].nombre)) {
    //    i++;
    //    if (!strncmp(nombre, texto, len)) return string_duplicate(nombre);
    //}
    return NULL;
}