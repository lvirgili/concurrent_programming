#ifndef LL_H
#define LL_H

/* Esse header define uma lista ligada simples que e usada na leitura
 * dos trechos da etapa. */
struct node {
     char t_trecho; /* Tipo do trecho. */
     int k; /* Distancia do trecho. */
     struct node *p; /* Proximo trecho. */
};

int ll_size; /* Armazena quantos nos existem na lista. */
struct node *head;

int ll_init();
int ll_add(char t, int dist);
int ll_clean();
void ll_print();

#endif
