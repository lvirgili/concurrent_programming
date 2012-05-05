#include "ep2_graph.hpp"

vector<unsigned long> arrive;
vector<int> order;
vector<vector<vector<int> > > paths;
once_flag flag;
atomic<int> l(0), itnumber(0);
int nrounds;
vector<pair<int,int> > limits;

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
     cout << "nrounds: " << nrounds << endl;
     for (unsigned i = 0; i < limits.size(); ++i) {
          cout << i << ": " << limits[i].first << ' ' << limits[i].second << endl;
     }
}

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

static void barrier(int tid, int nthreads) {
     while (atomic_fetch_add(&l, 1) > 0) {
          this_thread::yield();
     }
     order.push_back(tid);
     l.store(0);
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

static void find_path(int tid, int nvertex, int nthreads, int cur_vertex, vector<int>& cur_path, vector<vector<bool> > adj, bool debug) {
     cur_path.push_back(cur_vertex);
     if (cur_vertex >= limits[tid].first && cur_vertex <= limits[tid].second) {
          // Achou um caminho pra alguem.
          paths[cur_vertex].push_back(cur_path);
          while (atomic_fetch_add(&l, 1) > 0) {
               this_thread::yield();
          }
          cout << "a thread " << tid << " achou um caminho: \n";
          for (unsigned i = 0; i < paths[cur_vertex].size(); ++i) {
               for (unsigned j = 0; j < paths[cur_vertex][i].size(); ++j) {
                    cout << paths[cur_vertex][i][j] << ' ';
               }
               cout << endl;
          }
          l.store(0);
          barrier(tid, nthreads);
     }
     for (int i = 1; i < nvertex; ++i) {
          if (adj[cur_vertex][i] != 0 && (int)count(cur_path.begin(), cur_path.end(), i) == 0) {
               find_path(tid, nvertex, nthreads, i, cur_path, adj, debug);
          }
     }
     cur_path.pop_back();
}

void gen_paths(int n, int tid, int nvertex, int nthreads, vector<vector<bool> > adj, bool debug) {
     for (int i = 1; i < nvertex; ++i) {
          vector<int> cur_path;
          cur_path.push_back(0);
          if (adj[0][i] != 0) {
               find_path(tid, nvertex, nthreads, i, cur_path, adj, debug);
          }
          barrier(tid, nthreads);
     }
}

void test(int tid, int nthreads, bool debug) {
     for (int i = 0; i < 3; ++i) {
          if (tid == 0 && debug == true) {
               // while (atomic_fetch_add(&l, 1) > 0) {
               //      this_thread::yield();
               // }
               itnumber.fetch_add(1);
               cout << "Iteracao " << itnumber << " da barreira: ";
               //l.store(0);
          }
          barrier(tid, nthreads);
          if (tid == 0 && debug == true) {
               // while (atomic_fetch_add(&l, 1) > 0) {
               //      this_thread::yield();
               //             }
               for (int i = (itnumber-1)*nthreads; i < (itnumber-1)*nthreads + nthreads; ++i) {
                    cout << order[i] << ' ';
               }
               cout << endl;
//               l.store(0);
          }
     }
}
