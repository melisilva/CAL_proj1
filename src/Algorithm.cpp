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
        if (!pair.first) {
            //will just pass by this node
            stops->appendPath(calculateDrivePath(last, pair.second));
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
    final->appendPath(pathStops[0]);
    for (int i = 1; i < pathStops.size(); i++) {
        auto stopPath = pathStops[i];
        if (stopPath->isCarOnly()) {
            //We didn't stop at this intermediary point, we can just go there
            final->appendPath(stopPath);
            continue;
        }

        Node<int> *lastTrueDestiny, *thisTrueDestiny;
        Parking<int> *lastPark, *thisPark;
        GeneralPath *lastSubPath = final->getLastSubPath();
        lastPark = dynamic_cast<Parking<int> *>(final->getLast());
        lastTrueDestiny = lastSubPath->getFirst();
        thisPark = dynamic_cast<Parking<int> *>(stopPath->getLast());
        thisTrueDestiny = dynamic_cast<MultiplePath *>(stopPath)->getLastSubPath()->getFirst();


        //COST TO PARK IN stopPath PARK TOO
        double firstCost = 0;
        GeneralPath *lastPathFromLastTrueDestinyToPark = dynamic_cast<MultiplePath *>(lastSubPath)->getLastSubPath();
        firstCost += getDistancesCost(dynamic_cast<MultiplePath *>(lastPathFromLastTrueDestinyToPark));
        firstCost += getCost(dynamic_cast<MultiplePath *>(stopPath));
        if (i < pathStops.size() - 1) {
            GeneralPath *pathFromThisParkToNextPark = dynamic_cast<MultiplePath *>(pathStops[i + 1])->getFirstSubPath();
            firstCost += getDistancesCost((dynamic_cast<MultiplePath *>(pathFromThisParkToNextPark)));
        }


        //COST NOT TO PARK IN stopPath (straight path)
        double secondCost = 0, parksDist = 0;
        double destiniesDist = lastTrueDestiny->calcNodeDistance(thisTrueDestiny);
        double thisDestinyToLastParkDist = thisTrueDestiny->calcNodeDistance(lastPark);
        double parkCost = lastPark->getPrice(lastPark->getUserTime() + thisPark->getUserTime());
        if (i < pathStops.size() - 1) {
            parksDist = lastPark->calcNodeDistance(pathStops[i + 1]->getLast());
        }
        secondCost = walkWeight * (destiniesDist + thisDestinyToLastParkDist) + parkWeight * parkCost + driveWeight * parksDist;

        if (firstCost < secondCost) {
            //Going on foot doesn't pay off (even with straight distances)
            final->appendPath(stopPath);
            continue;
        }

        //COST NOT TO PARK IN stopPath
        //Now need to calculate the actual pathDist
        double thirdCost = 0, parksDistCost = 0;
        GeneralPath *parksPath;
        GeneralPath *destiniesPath = calculateWalkPath(lastTrueDestiny, thisTrueDestiny);
        GeneralPath *thisDestinyToLastParkPath = calculateWalkPath(thisTrueDestiny, lastPark);
        if (i < pathStops.size() - 1) {
            parksPath = calculateDrivePath(lastPark, pathStops[i + 1]->getLast());
            parksDistCost = getDistancesCost(parksPath);
        }
        thirdCost = getDistancesCost(destiniesPath) + getDistancesCost(thisDestinyToLastParkPath) + parkCost + parksDistCost;

        if (thirdCost < firstCost) {
            MultiplePath *multipleLastSubPath = dynamic_cast<MultiplePath *>(lastSubPath);
            multipleLastSubPath->removeLastSubPath();
            multipleLastSubPath->appendPath(destiniesPath);
            multipleLastSubPath->appendPath(thisDestinyToLastParkPath);
        }
    }
    return final;

}


GeneralPath *Algorithm::calculateBestPark(Node<int> *from, Node<int> *to, int time) {
    //P1 best park for walking
    //P2 best park for driving
    //P3 best park for price
    GeneralPath *pathBetweenFromP1 = graph.getNextClosestParking(from, true);
    GeneralPath *pathBetweenFromP2 = calculateDrivePath(from, calculateCheapestPark(time));
    GeneralPath *pathBetweenP3To = graph.getNextClosestParking(to, true);
    GeneralPath *pathBetweenFromP3 = calculateDrivePath(from, pathBetweenP3To->getFirst());
    dynamic_cast<Path *>(pathBetweenFromP1)->setCarOnly(true);
    dynamic_cast<Path *>(pathBetweenP3To)->setCarOnly(true);
    graph.initializeForShortestPath();
    sort(parkingNodes.begin(), parkingNodes.end(), [to](Node<int> *n1, Node<int> *n2) {
        return to->calcNodeDistance(n1) < to->calcNodeDistance(n2);
    });
    MultiplePath *minimumPath = new MultiplePath(from);
    minimumPath->appendPath(pathBetweenFromP3);
    minimumPath->appendPath(pathBetweenP3To);
    double minimumCost = getDistancesCost(pathBetweenFromP3)
                         + dynamic_cast<Parking<int> *>(pathBetweenFromP3->getLast())->getPrice(time)
                         + getDistancesCost(pathBetweenP3To);

    cout << "Working till here." << endl;

    double thisCost = 0;
    GeneralPath *pathBetweenFromNewPark;
    GeneralPath *pathBetweenNewParkTo;
    for (int j = 0; j < parkingNodes.size(); j++) {

        thisCost = driveWeight * pathBetweenFromP2->getLength()
                   + parkWeight * dynamic_cast<Parking<int> *>(pathBetweenP3To->getFirst())->getPrice(time)
                   + walkWeight * 2 * to->calcNodeDistance(parkingNodes[j]);
        if (thisCost > minimumCost) {
            //parar algoritmo
            break;
        }

        thisCost = driveWeight * (from->calcNodeDistance(parkingNodes[j]))
                   + parkWeight * dynamic_cast<Parking<int> *>(parkingNodes[j])->getPrice(time)
                   + walkWeight * 2 * to->calcNodeDistance(parkingNodes[j]);
        if (thisCost > minimumCost) {
            // podemos saltar
            continue;
        }

        pathBetweenNewParkTo = calculateWalkPath(parkingNodes[j], to);
        thisCost = driveWeight * (from->calcNodeDistance(parkingNodes[j]))
                   + parkWeight * dynamic_cast<Parking<int> *>(parkingNodes[j])->getPrice(time)
                   + walkWeight * 2 * pathBetweenNewParkTo->getLength();
        if (thisCost > minimumCost) {
            // podemos saltar
            continue;
        }

        pathBetweenFromNewPark = calculateDrivePath(from, parkingNodes[j]);
        thisCost = driveWeight * pathBetweenFromNewPark->getLength();
        +parkWeight * dynamic_cast<Parking<int> *>(parkingNodes[j])->getPrice(time)
        + walkWeight * 2 * pathBetweenNewParkTo->getLength();
        if (thisCost > minimumCost) {
            // podemos saltar
            continue;
        }
        minimumCost = thisCost;
        minimumPath = new MultiplePath(from);
        minimumPath->appendPath(pathBetweenFromNewPark);
        minimumPath->appendPath(pathBetweenNewParkTo);
        minimumPath->appendPath(pathBetweenNewParkTo->reverse());
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

GeneralPath *Algorithm::calculateDrivePath(Node<int> *from, Node<int> *to) {
    Path *p = dynamic_cast<Path *>(graph.aStarShortestPath(reinterpret_cast<int &>(from), reinterpret_cast<int &>(to)));
    p->setCarOnly(true);
    return p;
}

GeneralPath *Algorithm::calculateWalkPath(Node<int> *from, Node<int> *to) {
    Path *p = dynamic_cast<Path *>(graph.aStarShortestPathwalking(reinterpret_cast<int &>(from), reinterpret_cast<int &>(to)));
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
    for (auto parkingNode: parkingNodesSet) {
        total += dynamic_cast<Parking<int> *>(parkingNode)->getUserCost();
    }
    return total * parkWeight;
}