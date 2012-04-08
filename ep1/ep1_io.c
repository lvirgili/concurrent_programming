#include "ep1_io.h"

/* Le o arquivo de entrada. */
int read_file(char *path) {
     FILE *f;
     char t;
     int k, flag, dist = 0;
     f = fopen(path, "r");
     if (f == NULL) {
          return -1;
     }
     fscanf(f, "%d", &m);
     fscanf(f, "%d", &n);
     fscanf(f, "%c", &v); /* Ler o '\n'. */
     fscanf(f, "%c", &v);
     fscanf(f, "%d", &d);
     fscanf(f, "%c", &t); /* '\n'... */
     ll_init();
     while (dist < d) {
          fscanf(f, "%c", &t);
          fscanf(f, "%d", &k);
          dist += k;
          flag = ll_add(t, k);
          if (flag != 0) {
               ll_clean();
               return -1;
          }
          fscanf(f, "%c", &t);
     }
     fclose(f);
     return 0;
}

/* Imprime as variaveis lidas do arquivo. */
void print_vars() {
     printf("m = %d\nn = %d\nv = %c\nd = %d\n",m,n,v,d);
}
