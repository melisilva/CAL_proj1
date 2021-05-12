//
// Created by henri on 11/05/2021.
//

#include "Algorithm.h"
#include "FileManagement.h"

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

void Algorithm::execute(Node<int> *start, vector<pair<bool, Node<int>*>> toVisit) {
    vector<pair<bool, Node<int>*>> stops;
    for(int i = 0; i < toVisit.size(); i++){
        auto pair = toVisit[i];
        if(pair.first){
            //will visit this node
            stops.push_back(pair);
        }
        else{
            Node<int>* from;
            if(i == 0){
                from = start;
            }else{
                from = toVisit[i-1].second;
            }
        stops.push_back(make_pair<bool, Node<int>*>(true,calculateBestPark(from, pair.second)));
        }
    }
}