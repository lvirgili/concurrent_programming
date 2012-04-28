#ifndef IO_H
#define IO_H

#include <iostream>
#include <cstdlib>
#include <cctype>
#include <cstring>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

void read_input(int argc, char **argv, int *n, int *nvertex, bool *debug, vector<vector<bool> > &adj);
#endif
