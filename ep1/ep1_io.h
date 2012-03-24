#ifndef IO_H
#define IO_H

/* Variaveis do problema. */
int m; /* Numero de ciclistas. */
int n; /* Largura da pista em numero de ciclistas. */
char v; /* Modo de gerar as velocidades: A == aleatorio, U ==
           uniforme. */
int d; /* Distancia da etapa em km. */

int read_file(char *path);
void print_vars();

#endif
