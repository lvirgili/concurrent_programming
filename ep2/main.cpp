#include <iostream>
#include "ep2_pq.hpp"
using namespace std;

const int n = 6;
const int inf = 10000;
vector<int> cst, prnt;
int adj[6][6] = { {-1, -1, 7, 2, 4, -1},
                  {-1, -1, 0, -1, -1, -1},
                  {-1, -1, -1, -1, 1, -1},
                  {-1, -1, -1, -1, 1, 3},
                  {-1, 4, -1, -1, -1, 1},
                  {-1, 2, -1, -1, -1, -1}};

void dijkstra() {
     for (int i = 0; i < n; ++i) {
          prnt.push_back(-1);
          cst.push_back(inf);
     }
     int o = 0;
     cst[o] = 0;
     prnt[o] = o;
     pqueue pq(n, cst, 4);
     pq.insert(o);
     while (pq.empty() == false) {
          int v = pq.getmin();
          for (int i = 0; i < n; ++i) {
               if (adj[v][i] != -1) {
                    if (cst[i] == inf) {
                         cst[i] = cst[v] + adj[v][i];
                         prnt[i] = v;
                         pq.insert(i);
                    } else {
                         if (cst[i] > cst[v] + adj[v][i]) {
                              cst[i] = cst[v] + adj[v][i];
                              prnt[i] = v;
                              pq.lower(i);
                         }
                    }
               }
          }
     }
}

int main() {
     dijkstra();
     cout << "V ";
     for (int i = 0; i < n; ++i) {
          cout << i << ' ';
     }
     cout << endl << "p ";
     for (int i = 0; i < n; ++i) {
          cout << prnt[i] << ' ';
     }
     cout << endl << "c ";
     for (int i = 0; i < n; ++i) {
          cout << cst[i] << ' ';
     }
     cout << endl;
     return 0;
}
