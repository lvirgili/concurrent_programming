#ifndef PQ
#define PQ

#include <vector>
using namespace std;

class pqueue {
private:
     int d, n;
     vector<int> pq, qp;
     const vector<int> &cst;

     void exch(int i, int j);
     void fixUp(int k);
     void fixDown(int k, int N);

public:
     pqueue(int N, const vector<int> &cst, int d = 4) :
          d(d), n(0), pq(N+1, 0), qp(N+1,0), cst(cst) {}
     bool empty() const;
     void insert(int v);
     int getmin();
     void lower(int k);
};

#endif
