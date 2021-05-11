#ifndef PROJECT_FILEMANAGEMENT_H
#define PROJECT_FILEMANAGEMENT_H


#include <string>
#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>
#include "Graph.h"
using namespace std;

template <class T>
void readNodesFile(Graph<T> &graph, string nodesfile);

template <class T>
void readEdgesFile(Graph<T> &graph, bool bidir, string edgesfile);



#endif //PROJECT_FILEMANAGEMENT_H
