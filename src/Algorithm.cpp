//
// Created by henri on 11/05/2021.
//

#include <algorithm>
#include <unordered_set>
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
    MultiplePath *final = new MultiplePath(stops->getFirst());
    vector<GeneralPath *> pathStops = dynamic_cast<MultiplePath *>(stops)->getPath();
    for (auto stopPath: pathStops) {
        if (stopPath->isCarOnly()) {
            //We didn't stop at this intermediary point, we can just go there
            final->appendPath(stopPath);
            continue;
        }
        if (final->isEmpty()) {
            //Path was empty, park improvement is absurd
            final->appendPath(stopPath);
            continue;
        }

        Node<int> *lastTrueDestiny;
        GeneralPath *lastSubPath = final->getLastSubPath();
        lastTrueDestiny = lastSubPath->getFirst();


        //COST TO PARK IN stopPath PARK TOO
        Parking<int> *lastPark = dynamic_cast<Parking<int> *>(lastSubPath->getLast());
        int lastParkTime = lastPark->getUserTime();

        //COST NOT TO PARK IN stopPath
    }
    //execute obtencao do melhor percurso

}


GeneralPath *Algorithm::calculateBestPark(Node<int> *from, Node<int> *to, int time) {
    GeneralPath *pathToP1 = graph.getNextClosestParking(from, true);
    GeneralPath *pathToP2 = calculateDrivePath(from, calculateCheapestPark(time));
    GeneralPath *pathToP3 = graph.getNextClosestParking(to, true);
    dynamic_cast<Path *>(pathToP1)->setCarOnly(true);
    dynamic_cast<Path *>(pathToP3)->setCarOnly(true);
    graph.initializeForShortestPath();
    //    find p1, p2 and p3
    cout << "working till here";
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
    return graph.aStarShortestPath(reinterpret_cast<int &>(from), reinterpret_cast<int &>(to));
}

GeneralPath *Algorithm::calculateDrivePath(Node<int> *from, Node<int> *to) {
    Path *p = dynamic_cast<Path *>(calculatePath(from, to));
    p->setCarOnly(true);
    return p;
}

GeneralPath *Algorithm::calculateWalkPath(Node<int> *from, Node<int> *to) {
    Path *p = dynamic_cast<Path *>(calculatePath(from, to));
    p->setWalkOnly(true);
    return p;
}

double Algorithm::getCost(MultiplePath *path) {
    return getParkingCost(path) + getDistancesCost(path);
}

double Algorithm::getDistancesCost(GeneralPath *path) {
    if (path->isCarOnly()) {
        return walkWeight * path->getLength();
    }
    if (path->isWalkOnly()) {
        return walkWeight * path->getLength();
    }
    //Needs to be a multiplePath
    double total = 0;
    for (auto singlePath: dynamic_cast<MultiplePath *>(path)->getPath()) {
        total += getDistancesCost(singlePath);
    }
    return total;
}

double Algorithm::getParkingCost(GeneralPath *path) {
    double total;
    vector<Node<int> *> nodes = path->getAllNodes();
    vector<Node<int> *> parkingNodes(nodes.size());
    copy_if(nodes.begin(), nodes.end(), parkingNodes.begin(), [](Node<int> *n) {
        return dynamic_cast<Parking<int> *>(n) != nullptr;
    });
    unordered_set<Node<int> *> parkingNodesSet; //hash and comparison not defined. could yield weird errors
    for (auto parkingNode: parkingNodes) {
        parkingNodesSet.insert(parkingNode);
    }
    for(auto parkingNode: parkingNodesSet){
        total += dynamic_cast<Parking<int>*>(parkingNode)->getUserCost();
    }
    return total;
}