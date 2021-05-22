//
// Created by henri on 11/05/2021.
//

#ifndef PROJECT_ALGORITHM_H
#define PROJECT_ALGORITHM_H


#include <string>
#include <chrono>
#include "Node.h"
#include "Graph.h"

using namespace std;
class GeneralPath;
class MultiplePath;


class Algorithm {
    Graph<int> graph;
    vector<Node<int> *> parkingNodes;
    float driveWeight = 0.24, parkWeight = 0.01, walkWeight = 0.75;
    long long int edgesReadingTime, nodesReadingTime, parksReadingTime;
public:
    Algorithm(string nodesFile, string edgesFile, string parkingFiles);

    Graph<int> getGraph() const;

    vector<Node<int> *> getParking() const;

    void setWeights(float a, float b, float c);

    void execute(Node<int> *start, vector<pair<int, Node<int> *>> toVisit);

    MultiplePath *calculateBestParkEachStop(Node<int> *start, vector<pair<int, Node<int> *>> toVisit);

    MultiplePath *calculateFinalPath(GeneralPath * stops);

    GeneralPath *calculateBestPark(Node<int> *from, Node<int> *to, int time);

    Node<int> * calculateCheapestPark(int time);

    GeneralPath *calculateDrivePath(Node<int> *from, Node<int> *to);

    GeneralPath *calculateWalkPath(Node<int> *from, Node<int> *to);

    double getCost(MultiplePath * path);

    double getDistancesCost(GeneralPath *path);

    double getParkingCost(GeneralPath *path);
};




#endif //PROJECT_ALGORITHM_H
