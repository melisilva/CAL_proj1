//
// Created by henri on 11/05/2021.
//

#include <algorithm>
#include <unordered_set>
#include <chrono>
#include "Algorithm.h"
#include "FileManagement.h"
#include "MultiplePath.h"
#include "Graph.h"
#include "ViewGraph.h"

Algorithm::Algorithm(string nodesFile, string edgesFile, string parkingFile) {
    auto startTime = std::chrono::high_resolution_clock::now();
    readNodesFile(graph, nodesFile);
    auto finishTime = std::chrono::high_resolution_clock::now();
    nodesReadingTime = std::chrono::duration_cast<std::chrono::microseconds>(finishTime - startTime).count();

    startTime =std::chrono::high_resolution_clock::now();
    readEdgesFile(graph, false, edgesFile);
    finishTime = std::chrono::high_resolution_clock::now();
    edgesReadingTime = std::chrono::duration_cast<std::chrono::microseconds>(finishTime - startTime).count();

    startTime =std::chrono::high_resolution_clock::now();
    parkingNodes = readParkingFile(graph, parkingFile);
    finishTime = std::chrono::high_resolution_clock::now();
    parksReadingTime = std::chrono::duration_cast<std::chrono::microseconds>(finishTime - startTime).count();
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

MultiplePath* Algorithm::execute(Node<int> *start, vector<pair<int, Node<int> *>> toVisit, long long int &elapsed) {
    auto startTime = std::chrono::high_resolution_clock::now();

    auto pair = toVisit[0];
    MultiplePath *stops = dynamic_cast<MultiplePath *>(calculateBestParkEachStop(start, toVisit));
    if (!stops->isEmpty()) {
        stops = calculateFinalPath(stops);
    }
    auto finishTime = std::chrono::high_resolution_clock::now();
    elapsed = std::chrono::duration_cast<std::chrono::microseconds>(finishTime - startTime).count();
    return stops;
}

MultiplePath * Algorithm::visualizeExecute(Node<int> *start, vector<pair<int, Node<int> *>> toVisit, long long int &elapsed) {
    MultiplePath* stops = execute(start, toVisit, elapsed);
    //stops->displayPath();
    cout<<"Total price of the trip is "<<total_price<<endl;
    total_price=0;
    fflush(stdout);
    showPath(stops->getAllNodes(), graph);
    cout << "Time used for reading nodes, edges and parks " << nodesReadingTime << " " << edgesReadingTime << " " << parksReadingTime << " microseconds\n";
    cout << "Time used for processing " << elapsed << " microseconds or " << elapsed/ 1000000.0 << " seconds\n";
}


MultiplePath *Algorithm::calculateBestParkEachStop(Node<int> *start, vector<pair<int, Node<int> *>> toVisit) {
    MultiplePath *stops = new MultiplePath(start);
    Node<int> *last = start;
    for (int i = 0; i < toVisit.size(); i++) {
        auto pair = toVisit[i];
        if (pair.first == 0) {
            //will just pass by this node without parking
            MultiplePath *pathToStop = new MultiplePath(last);
            pathToStop->appendPath(calculateDrivePath(last, pair.second));
            stops->appendPath(pathToStop);
            last = pair.second;
        } else {
            GeneralPath *toParkInterPark = calculateBestPark(last, pair.second, pair.first);
            if (toParkInterPark->isEmpty()) {
                //Cannot park on that stop, probably means that the stop is disconnected from the rest of the graph
                // Just ignore it and proceed to the next one
                continue;
            }
            stops->appendPath(toParkInterPark);
            toParkInterPark->getLast()->getParkingNode()->setUserWillStop(true);

            //Another strategy might take last = pair.second and use multithreading
            last = toParkInterPark->getLast();

        }
    }
    return stops;
}

MultiplePath *Algorithm::calculateFinalPath(GeneralPath *stops) {
    MultiplePath *final = new MultiplePath(stops->getFirst());
    vector<GeneralPath *> pathStops = dynamic_cast<MultiplePath *>(stops)->getPath();
    final->appendPath(pathStops[0]); //Must go to the first destiny
    for (int i = 1; i < pathStops.size(); i++) {
        auto stopPath = pathStops[i];
        if (stopPath->isCarOnly()) {
            //We didn't stop at this intermediary point, we can just go there
            final->appendPath(stopPath);
            continue;
        }

        GeneralPath *lastSubPath = final->getLastSubPath();

        if(lastSubPath->isCarOnly()){
            final->appendPath(stopPath);
            continue;
        }
        Node<int> *lastTrueDestiny, *thisTrueDestiny;
        Node<int> *lastPark, *thisPark;
        double lastParkTime, lastParkCost, thisParkTime;

        lastPark = final->getLast();
        lastParkTime = lastPark->getParkingNode()->getUserTime();
        lastTrueDestiny = lastSubPath->getFirst();
        thisPark = stopPath->getLast();
        thisParkTime = thisPark->getParkingNode()->getUserTime();
        thisTrueDestiny = dynamic_cast<MultiplePath *>(stopPath)->getLastSubPath()->getFirst();


        //COST TO PARK IN stopPath PARK TOO
        double firstCost = 0;
        GeneralPath *lastPathFromLastTrueDestinyToPark = dynamic_cast<MultiplePath *>(lastSubPath)->getLastSubPath();
        firstCost += getDistancesCost(lastPathFromLastTrueDestinyToPark);
        firstCost += getCost(dynamic_cast<MultiplePath *>(stopPath));
        if (i < pathStops.size() - 1) {
            GeneralPath *pathFromThisParkToNextPark = dynamic_cast<MultiplePath *>(pathStops[i + 1])->getFirstSubPath();
            firstCost += getDistancesCost(pathFromThisParkToNextPark);
        }


        //COST NOT TO PARK IN stopPath (straight path)
        double secondCost = 0, parksDist = 0;
        double destiniesDist = lastTrueDestiny->calcNodeDistance(thisTrueDestiny);
        double thisDestinyToLastParkDist = thisTrueDestiny->calcNodeDistance(lastPark);
        double parkCost = lastPark->getParkingNode()->getPrice(lastParkTime + thisParkTime);
//        double parkCost = 0;
//        if (lastPark->getParking()) {
//            parkCost = lastPark->getParkingNode()->getPrice(lastPark->getParkingNode()->getUserTime() + thisPark->getParkingNode()->getUserTime());
//        }
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

            lastPark->getParkingNode()->setUserTime(0);
            lastPark->getParkingNode()->setUserWillStop(false);
            thisPark->getParkingNode()->setUserTime(lastParkTime + thisParkTime);
        }
    }
    return final;

}


GeneralPath *Algorithm::calculateBestPark(Node<int> *from, Node<int> *to, int time) {
    //TODO bool to see if it is the destiny and walking distance is halved
    //P1 best park for walking
    //P2 best park for driving
    //P3 best park for price
    bool gettingFirstParking = true;
    GeneralPath *pathBetweenP1To;
    GeneralPath *pathBetweenFromP1;
    while (1) {
        pathBetweenP1To = graph.getNextClosestParking(to, gettingFirstParking, true);
        if (pathBetweenP1To->isEmpty()) {
            // The to cannot get to any park on foot
            return pathBetweenP1To;
        }
        pathBetweenFromP1 = calculateDrivePath(from, pathBetweenP1To->getFirst());
        if (pathBetweenFromP1->isEmpty()) {
            // Cannot get to that park from the source
            // Will try to get the next closest parking on foot
            gettingFirstParking = false;
            continue;
        }
        break;
    }
    Node<int> *P3 = calculateCheapestPark(time);
    GeneralPath *pathBetweenP2From = graph.getNextClosestParking(to, true, false);
    if (pathBetweenP2From->isEmpty()) {
        // Cannot get to any park by driving from the source
        return pathBetweenP2From;
    }
    GeneralPath *pathBetweenFromP2 = pathBetweenP2From->reverse();
//    GeneralPath *pathBetweenFromP2 = calculateDrivePath(from, pathBetweenP2From->getFirst()); //getNextClosestParking might be upgraded to bidirectional. Cant just reverse
    dynamic_cast<Path *>(pathBetweenP1To)->setWalkOnly(true);
    dynamic_cast<Path *>(pathBetweenFromP2)->setCarOnly(true);

    graph.initializeForShortestPath(); //Reset changes to vertex variables

    sort(parkingNodes.begin(), parkingNodes.end(), [to](Node<int> *n1, Node<int> *n2) {
        return to->calcNodeDistance(n1) < to->calcNodeDistance(n2);
    });
    MultiplePath *minimumPath = new MultiplePath(from);
    minimumPath->appendPath(pathBetweenFromP1);
    minimumPath->appendPath(pathBetweenP1To);
    minimumPath->appendPath(pathBetweenP1To->reverse());
    double minimumCost = getDistancesCost(pathBetweenFromP1)
                         + (pathBetweenFromP2->getLast())->getParkingNode()->getPrice(time)
                         + getDistancesCost(pathBetweenP1To) * 2;

    double thisCost = 0;
    GeneralPath *pathBetweenFromNewPark;
    GeneralPath *pathBetweenNewParkTo;
    for (int j = 0; j < parkingNodes.size(); j++) {

        thisCost = driveWeight * pathBetweenFromP2->getLength()
                   + parkWeight * P3->getParkingNode()->getPrice(time)
                   + walkWeight * 2 * to->calcNodeDistance(parkingNodes[j]);
        if (thisCost > minimumCost) {
            //parar algoritmo
            break;
        }

        thisCost = driveWeight * (from->calcNodeDistance(parkingNodes[j]))
                   + parkWeight * (parkingNodes[j])->getParkingNode()->getPrice(time)
                   + walkWeight * 2 * to->calcNodeDistance(parkingNodes[j]);
        if (thisCost > minimumCost) {
            // podemos saltar
            continue;
        }

        pathBetweenNewParkTo = calculateWalkPath(parkingNodes[j], to);
        if (pathBetweenNewParkTo->isEmpty()) {
            //Couldn't walk to this park from to
            continue;
        }
        thisCost = driveWeight * (from->calcNodeDistance(parkingNodes[j]))
                   + parkWeight * (parkingNodes[j])->getParkingNode()->getPrice(time)
                   + walkWeight * 2 * pathBetweenNewParkTo->getLength();
        if (thisCost > minimumCost) {
            // podemos saltar
            continue;
        }

        pathBetweenFromNewPark = calculateDrivePath(from, parkingNodes[j]);
        if (pathBetweenFromNewPark->isEmpty()) {
            // Couldn't drive to this park from source
            continue;
        }
        thisCost = driveWeight * pathBetweenFromNewPark->getLength();
        +parkWeight * (parkingNodes[j])->getParkingNode()->getPrice(time)
        + walkWeight * 2 * pathBetweenNewParkTo->getLength();
        if (thisCost > minimumCost) {
            // podemos saltar
            continue;
        }
        minimumCost = thisCost;
        total_price+=minimumCost;
        minimumPath = new MultiplePath(from);
        minimumPath->appendPath(pathBetweenFromNewPark);
        minimumPath->appendPath(pathBetweenNewParkTo);
        minimumPath->appendPath(pathBetweenNewParkTo->reverse());
    }
    return minimumPath;
}

Node<int> *Algorithm::calculateCheapestPark(int time) {
    Node<int> *cheapest = *std::min_element(
            parkingNodes.begin(),
            parkingNodes.end(),
            [time](Node<int> *v1, Node<int> *v2) {
                return v1->getParkingNode()->getPrice(time) < v2->getParkingNode()->getPrice(time);
            }
    );
    //DEBUG
//    auto set = graph.getVertexSet();
//    auto cheapest2 = *find_if(set.begin(), set.end(), [](Vertex<int>*n1){return n1->getInfo() == 26016895;});
//    cheapest = dynamic_cast<Node<int>*>(cheapest2);
    return cheapest;
}

GeneralPath *Algorithm::calculateDrivePath(Node<int> *from, Node<int> *to) {
    Path *p = dynamic_cast<Path *>(graph.aStarShortestPath(from->getInfo(), to->getInfo()));
    p->setCarOnly(true);
    return p;
}

GeneralPath *Algorithm::calculateWalkPath(Node<int> *from, Node<int> *to) {
    Path *p = dynamic_cast<Path *>(graph.aStarShortestPathwalking(from->getInfo(), to->getInfo()));
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
    auto it = copy_if(nodes.begin(), nodes.end(), parkingNodes.begin(), [](Node<int> *n) {
        return n->getParking();
    });
    parkingNodes.resize(distance(parkingNodes.begin(), it));
    unordered_set<Node<int> *> parkingNodesSet; //hash and comparison not defined. could yield weird errors
    for (auto parkingNode: parkingNodes) {
        parkingNodesSet.insert(parkingNode);
    }
    for (auto parkingNode: parkingNodesSet) {
        total += (parkingNode)->getParkingNode()->getUserCost();
    }
    return total * parkWeight;
}