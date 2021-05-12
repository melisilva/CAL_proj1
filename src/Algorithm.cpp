//
// Created by henri on 11/05/2021.
//

#include <algorithm>
#include "Algorithm.h"
#include "FileManagement.h"
#include "MultiplePath.h"

Algorithm::Algorithm(string nodesFile, string edgesFile, string parkingFile) {
    readNodesFile(graph, nodesFile);
    readEdgesFile(graph, false, edgesFile);
    parkingNodes = readParkingFile(graph, parkingFile);
}

Graph<int> Algorithm::getGraph() const {
    return graph;
}

vector<Node<int> *> Algorithm::getParking() const {
    return parkingNodes;

}

void Algorithm::setWeights(float a, float b, float c) {
    driveWeight = a;
    parkWeight = b;
    walkWeight = c;
}

void Algorithm::execute(Node<int> *start, vector<pair<int, Node<int> *>> toVisit) {

    MultiplePath *stops = dynamic_cast<MultiplePath *>(calculateBestParkEachStop(start, toVisit));
    stops = calculateFinalPath(stops);
    stops->displayPath();

}


MultiplePath *Algorithm::calculateBestParkEachStop(Node<int> *start, vector<pair<int, Node<int> *>> toVisit) {
    MultiplePath *stops;
    Node<int> *last = start;
    for (int i = 0; i < toVisit.size(); i++) {
        auto pair = toVisit[i];
        if (pair.first) {
            //will visit this node
            stops->appendPath(calculatePath(last, pair.second));
            last = pair.second;
        } else {
            GeneralPath *toParkInterPark = calculateBestPark(last, pair.second, pair.first);
            stops->appendPath(toParkInterPark);

            //Another strategy might take last = pair.second and use multithreading
            last = toParkInterPark->getLast();
        }
    }
    return stops;
}

MultiplePath *Algorithm::calculateFinalPath(GeneralPath *stops) {
    //execute obtencao do melhor percurso
}


GeneralPath *Algorithm::calculateBestPark(Node<int> *from, Node<int> *to, int time) {
    GeneralPath *pathToP1 = graph.getNextClosestParking(from, true);
    GeneralPath *pathToP2 = calculatePath(from, calculateCheapestPark(time));
    GeneralPath *pathToP3 = graph.getNextClosestParking(to, true);
    graph.initializeForShortestPath();
    //    find p1, p2 and p3
    cout <<"working till here";
    //execute point 1
}

Node<int> *Algorithm::calculateCheapestPark(int time) {
    auto vertexSet = graph.getVertexSet();
    std::min_element(
            vertexSet.begin(),
            vertexSet.end(),
            [time](Vertex<int> *v1, Vertex<int> *v2) {
                return dynamic_cast<Parking<int> *>(v1)->getPrice(time) < dynamic_cast<Parking<int> *>(v2)->getPrice(time);
            }
    );
}

GeneralPath *Algorithm::calculatePath(Node<int> *from, Node<int> *to) {
//TODO
}