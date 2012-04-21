#include "ep2_pq.hpp"

void pqueue::exch(int i, int j) {
     int t = pq[i];
     pq[i] = pq[j];
     pq[j] = t;
     qp[pq[i]] = i;
     qp[pq[j]] = j;
}

void pqueue::fixUp(int k) {
     while (k > 1 && cst[pq[(k+d-2)/d]] > cst[pq[k]]) {
          exch(k, (k+d-2)/d);
          k = (k+d-2)/d;
     }
}

void pqueue::fixDown(int k, int N) {
     int j = d*(k-1) + 2;
     while (j <= N) {
          for (int i = j+1; i < j+d && i <= N; ++i) {
               if (cst[pq[j]] > cst[pq[i]]) {
                    j = i;
               }
          }
          if (!(cst[pq[k]] > cst[pq[j]])) {
               break;
          }
          exch(k, j);
          k = j;
     }
}

bool pqueue::empty() const {
     return (n == 0);
}

void pqueue::insert(int v) {
     ++n;
     pq[n] = v;
     qp[v] = n;
     fixUp(n);
}

int pqueue::getmin() {
     exch(1, n);
     fixDown(1, n-1);
     return pq[n--];
}

void pqueue::lower(int k) {
     fixUp(qp[k]);
}
