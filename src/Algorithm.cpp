//
// Created by henri on 11/05/2021.
//

#include <algorithm>
#include "Algorithm.h"
#include "FileManagement.h"
#include "MultiplePath.h"
#include "Graph.h"

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
    sort(parkingNodes.begin(), parkingNodes.end(), [to](Node<int> *n1, Node<int> *n2){
        return to->calcNodeDistance(n1) < to->calcNodeDistance(n2);
    });

    cout << "Working till here." << endl;

    for (int j = 0; j < parkingNodes.size(); j++){
        if (driveWeight*pathToP2->getLength() + parkWeight*dynamic_cast<Parking<int>*>(pathToP3->getLast())->getPrice(time) + walkWeight*2*to->calcNodeDistance(parkingNodes[j]) > 0 /*minimum*/ ){
            //parar algoritmo
            break;
        } else if (driveWeight*(from->calcNodeDistance(parkingNodes[j])) + parkWeight*dynamic_cast<Parking<int>*>(parkingNodes[j])->getPrice(time) + walkWeight*2*to->calcNodeDistance(parkingNodes[j]) > 0 /*minimum*/ ) {
            // podemos saltar
            continue;
        } else if (driveWeight*(from->calcNodeDistance(parkingNodes[j])) + parkWeight*dynamic_cast<Parking<int>*>(parkingNodes[j])->getPrice(time) + walkWeight*2*calculatePath(parkingNodes[j], to)->getLength() > 0 /*minimum*/ ){
            // podemos saltar
            continue;
        } else {
            if (driveWeight*(from->calcNodeDistance(parkingNodes[j])) + parkWeight*dynamic_cast<Parking<int>*>(parkingNodes[j])->getPrice(time) + walkWeight*2*calculatePath(parkingNodes[j], to)->getLength() < 0 /*minimum*/){
                //o que está na esquerda torna-se o novo mínimo.
            }
        }
    }

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
   return graph.aStarShortestPath(reinterpret_cast<int &>(from), reinterpret_cast<int &>(to));
}