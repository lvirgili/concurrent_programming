#include "ep1_ll.h"
#include <stdlib.h>
#include <stdio.h>

/* Inicializa a lista. */
int ll_init() {
     head = NULL;
     ll_size = 0;
     return 0;
}

/* Adiciona um noh no fim da lista. */
int ll_add(char t, int dist) {
     struct node *pos = head;
     int i = 1;
     struct node *new = malloc(sizeof(*new));
     if (new == NULL) {
          return 1;
     }
     new->t_trecho = t;
     new->k = dist;
     new->p = NULL;
     if (ll_size == 0) {
          head = new;
     } else {
          while (i < ll_size) {
               pos = pos->p;
               ++i;
          }
          pos->p = new;
     }
     ++ll_size;
     return 0;
}

/* Desaloca a lista. */
int ll_clean() {
     struct node *aux;
     while (ll_size > 0) {
          aux = head;
          head = head->p;
          free(aux);
          --ll_size;
     }
     return 0;
}

/* Imprime a lista. */
void ll_print() {
     struct node *p = head;
     printf("Size = %d\n", ll_size);
     while (p != NULL) {
          printf("%c %d -> ", p->t_trecho, p->k);
          p = p->p;
     }
     printf("NULL\n");
}
