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
    float driveWeight, parkWeight, walkWeight;
public:
    Algorithm(string nodesFile, string edgesFile, string parkingFiles);

    Graph<int> getGraph() const;

    vector<Node<int> *> getParking() const;

    void setWeights(float a, float b, float c);

    void execute(Node<int> *start, vector<pair<bool, Node<int>*>>);

    Node<int> *calculateBestPark(Node<int>* from, Node<int> *to);
};


#endif //PROJECT_ALGORITHM_H
