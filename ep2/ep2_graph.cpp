#include "ep2_graph.hpp"

vector<unsigned long> arrive;
vector<int> order;
vector<vector<vector<int> > > paths;
atomic<int> itnumber(0);
int nrounds;
vector<pair<int,int> > limits;
mutex trava;

// Funcao que inicializa as variaveis necessarias. Nela tambem e
// calculado o numero de rodadas que a barreira vai precisar na linha
// 27.
void init(int nthreads, int nvertex) {
     int block_size = (nvertex - 1) / nthreads;
     int additions = ((nvertex - 1) % nthreads) - 1;
     int start = 1;
     for (int i = 0; i < nthreads; ++i) {
          arrive.push_back(0);
          pair<int,int> aux;
          limits.push_back(make_pair(start, start + block_size));
          if (i > additions) {
               --limits[i].second;
          }
          start = limits[i].second + 1;
     }
     nrounds = ceil(log(nthreads) / log(2));
     for (int i = 0; i < nvertex; ++i) {
          vector<vector<int> > aux;
          paths.push_back(aux);
     }
}

// Compara os vetores que armazenam o caminho. Utilizada pelo sort.
static bool comp_vectors(vector<int> a, vector<int> b) {
     if (a.empty() == true) return false;
     if (b.empty() == true) return true;
     return a.size() < b.size();
}

// Funcao potencia para inteiros que funciona em O(logn) ao inves de
// ser linear.
static int power2to(int k) {
     if (k == 0) {
          return 1;
     }
     if (k % 2 == 0) {
          return power2to(k/2) * power2to(k/2);
     } else {
          return 2 * power2to(k/2) * power2to(k/2);
     }
}

// Como o nome diz, imprime o que foi pedido. Se a opcao debug estiver
// ativada imprime sempre que ha sincronizacao.
static void print_stuff(int n, int nthreads, bool debug, bool end) {
     for (unsigned i = 1; i < paths.size(); ++i) {
          if (paths[i].empty() != false) {
               sort(paths[i].begin(), paths[i].end(), comp_vectors);
          }
     }
     if (debug == true) {
          cout << "Iteracao " << itnumber << " da barreira.\n";
          cout << "Ordem com que as threads chegaram na barreira: ";
          for (int i = (itnumber-1)*nthreads; i < itnumber*nthreads; ++i) {
               cout << order[i] << ' ';
          }
          cout << endl;
     }
     if (end == true) {
          cout << "Iteracao " << itnumber << " da barreira.\n";
     }
     if (debug == true || end == true) {
          for (unsigned i = 1; i < paths.size(); ++i) {
               cout << "Para o vertice " << i << ":\n";
               for (unsigned j = 0; j < paths[i].size(); ++j) {
                    if (j >= (unsigned)n) break;
                    cout << '(' << paths[i][j].size()-1 << ") ";
                    for (unsigned k = 0; k < paths[i][j].size(); ++k) {
                         cout << paths[i][j][k];
                         if (k != paths[i][j].size()-1) cout << " - ";
                    }
                    cout << endl;
               }
               cout << endl;
          }
          cout << endl;
     }
}

// Aqui esta a barreira. Ela e uma barreira de propagacao.
static void barrier(int tid, int nthreads) {
     trava.lock();
     if (tid == nthreads-1) {
          itnumber.fetch_add(1);
     }
     order.push_back(tid);
     trava.unlock();
     int sid;
     for (int i = 1; i <= nrounds; ++i) {
          ++arrive[tid];
          // Determina quem a thread vai esperar.
          sid = (tid + power2to(i-1)) % nthreads;
          while (arrive[sid] < arrive[tid]) {
               this_thread::yield();
          }
     }
}

// Funcao que faz as buscas em profundidade que as threads
// executam. Aqui ocorrem as sincronizacoes.
static void find_path(int n, int tid, int nvertex, int nthreads, int cur_vertex, vector<int>& cur_path, vector<vector<bool> > adj, bool debug) {
     cur_path.push_back(cur_vertex);
     if (cur_vertex >= limits[tid].first && cur_vertex <= limits[tid].second) {
          // Achou um caminho pra alguem.
          vector<int> aux; paths[cur_vertex].push_back(aux);
          for (unsigned i = 0; i < cur_path.size(); ++i) {
               paths[cur_vertex][paths[cur_vertex].size()-1].push_back(cur_path[i]);
          }
          if (tid == nthreads-1) {
               print_stuff(n, nthreads, debug, false);
          }
     }
     barrier(tid, nthreads);
     if (tid == nthreads-1) {
          print_stuff(n, nthreads, debug, false);
     }
     for (int i = 1; i < nvertex; ++i) {
          if (adj[cur_vertex][i] != 0 && (int)count(cur_path.begin(), cur_path.end(), i) == 0) {
               find_path(n, tid, nvertex, nthreads, i, cur_path, adj, debug);
          }
     }
     cur_path.pop_back();
}

// Funcao que as threads comecam executando. A "casca da bfs", nas
// palavras do Prof. Coelho. No final, a thread com o tid mais alto
// imprime o que falta.
void gen_paths(int n, int tid, int nvertex, int nthreads, vector<vector<bool> > adj, bool debug) {
     for (int i = 1; i < nvertex; ++i) {
          vector<int> cur_path;
          cur_path.push_back(0);
          if (adj[0][i] != 0) {
               find_path(n, tid, nvertex, nthreads, i, cur_path, adj, debug);
          }
     }
     if (tid == nthreads-1) {
          print_stuff(n, nthreads, false, true);
     }
}
