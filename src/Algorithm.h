//
// Created by henri on 11/05/2021.
//

#ifndef PROJECT_ALGORITHM_H
#define PROJECT_ALGORITHM_H


#include <string>
#include "Graph.h"

using namespace std;
class GeneralPath;
class MultiplePath;


class Algorithm {
    Graph<int> graph;
    vector<Node<int> *> parkingNodes;
    float driveWeight, parkWeight, walkWeight;
public:
    Algorithm(string nodesFile, string edgesFile, string parkingFiles);

    Graph<int> getGraph() const;

    vector<Node<int> *> getParking() const;

    void setWeights(float a, float b, float c);

    void execute(Node<int> *start, vector<pair<bool, Node<int> *>> toVisit);

    GeneralPath *calculateBestPark(Node<int> *from, Node<int> *to, int time);

    GeneralPath *calculatePath(Node<int> *from, Node<int> *to);

    MultiplePath *calculateBestParkEachStop(Node<int> *start, vector<pair<int, Node<int> *>> toVisit);

    MultiplePath *calculateFinalPath(GeneralPath * stops);
};


#endif //PROJECT_ALGORITHM_H
