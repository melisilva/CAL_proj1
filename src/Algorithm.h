//
// Created by henri on 11/05/2021.
//

#ifndef PROJECT_ALGORITHM_H
#define PROJECT_ALGORITHM_H


#include <string>
#include "graph.h"
using namespace std;


class Algorithm {
    Graph<int> graph;
    vector<Node<int> *> parkingNodes;
public:
        Algorithm(string nodesFile, string edgesFile, string parkingFiles);
};


#endif //PROJECT_ALGORITHM_H
