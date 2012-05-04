#include "ep2_graph.hpp"

vector<unsigned long> arrive;
once_flag flag;
atomic<int> l(0);
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
     int sid;
     for (int i = 1; i <= nrounds; ++i) {
          ++arrive[tid];
          // Determina quem a thread vai esperar.
          sid = (tid + power2to(i-1)) % nthreads;
          while (atomic_fetch_add(&l, 1) > 0) {
               this_thread::yield();
          }
          cout << "thread " << tid << " na fase " << i << " esperando " << sid << endl;
          l.store(0);
          while (arrive[sid] < arrive[tid]) {
               this_thread::yield();
          }
     }
     //call_once(flag, []() { stage.fetch_add(1); cout << "======= " << stage << " =======\n"; });
}

void test(int tid, int nthreads) {
     for (int i = 0; i < 5; ++i) {
          barrier(tid, nthreads);
     }
}
