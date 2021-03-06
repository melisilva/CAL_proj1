//
// Created by henri on 12/05/2021.
//

#ifndef PROJECT_NODE_H
#define PROJECT_NODE_H

#include <iostream>

using namespace std;

#include <cmath>
#include "Vertex.h"
#include "Parking.h"

template<class T>
class Node : public Vertex<T> {

    double lat, longi;
    bool isParking = false;
    Parking *parking = nullptr;
public:

    Node(int id, double lat, double longi) : Vertex<T>(id), lat(lat), longi(longi) {}

    void displayNode() const {
        cout << Vertex<T>::info << ' ' << lat << ' ' << longi << '\n';
    }

    double getLatitude() const {
        return lat;
    }

    double getLongitude() const {
        return longi;
    }

    bool getParking() const {
        return isParking;
    }

    void setParking() {
        isParking = true;
    }

    void setParking(bool isParking) {
        this->isParking = isParking;
    }

    double getHeuristic(Vertex<T> *v) {
        Node<T> *node = dynamic_cast<Node *>(v);
        return sqrt(pow((getLatitude() - node->getLatitude()), 2) + pow((getLongitude() - node->getLongitude()), 2));
    }

    double calcNodeDistance(Node<T> *a) {
        return sqrt(pow(a->lat - lat, 2) + pow(a->longi - longi, 2));
    }

    bool operator==(Node<T> *a) {
        if (lat == a->getLatitude() && longi == a->getLongitude()) {
            return true;
        }
        return false;
    }

    void setParkingNode(Parking *park){
        parking = park;
    }

    Parking* getParkingNode(){
        return parking;
    }

};


#endif //PROJECT_NODE_H