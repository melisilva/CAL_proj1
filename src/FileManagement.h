#ifndef PROJECT_FILEMANAGEMENT_H
#define PROJECT_FILEMANAGEMENT_H


#include <string>
#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <stdlib.h>
#include "Node.h"
#include "Graph.h"
#include "Parking.h"
#include "FixedPriceParking.h"
#include "DynamicPriceParking.h"
using namespace std;
template<class T>
void readNodesFile(Graph<T> &graph, string nodesfile) {
    //node file
    ifstream nodes;
    string line;
    stringstream ss;
    int numNodes;

    nodes.open(nodesfile);

    getline(nodes, line);
    ss << line;
    ss >> numNodes;

    for (int i = 0; i < numNodes; i++) {
        getline(nodes, line);
        int id;
        double x, y;
        size_t pos = line.find(',');
        ss.clear();
        ss << line.substr(1, pos);
        ss >> id;
        line.erase(0, pos + 1);
        pos = line.find(',');
        ss.clear();
        ss << line.substr(0, pos);
        ss >> x;
        line.erase(0, pos + 1);
        pos = line.find(')');
        ss.clear();
        ss << line.substr(0, pos);
        ss >> y;
        Node<T> *nodeToAdd = new Node<T>(id, x, y);
        graph.addVertex(nodeToAdd);

    }
    nodes.close();
}

template<class T>
void readEdgesFile(Graph<T> &graph, bool bidir, string edgesfile) {
    //edges file
    ifstream edges;
    string line;
    stringstream ss;
    int numEdges;

    edges.open(edgesfile);

    getline(edges, line);
    ss << line;
    ss >> numEdges;
    for (int i = 0; i < numEdges; i++) {
        getline(edges, line);
        int n1, n2;
        size_t pos = line.find(',');
        ss.clear();
        ss << line.substr(1, pos);
        ss >> n1;
        line.erase(0, pos + 1);
        pos = line.find(')');
        ss.clear();
        ss << line.substr(0, pos);
        ss >> n2;

        Vertex<T> *v1 = graph.findVertex(n1);
        Vertex<T> *v2 = graph.findVertex(n2);
        double weight = graph.heuristic(v1, v2);
        if (bidir)
            graph.addBiEdge(v1->getInfo(), v2->getInfo(), weight);
        else
            graph.addEdge(v1->getInfo(), v2->getInfo(), 0, weight, 0);
        //capacity and flow aren't used for our problem
    }
    edges.close();

}

template<class T>
vector<Node<T> *> readParkingFile(Graph<T> &graph, string parkingFile) {
    //parking file
    ifstream parkingsFile;
    string line;
    char aux;
    int noParkings, id;
    vector<Node<T> *> parkingNodes;

    parkingsFile.open(parkingFile);

    parkingsFile >> noParkings;
    parkingsFile.get(aux);
    getline(parkingsFile, line); // amenity=car_parking
    for (int i = 0; i < noParkings; i++) {
        parkingsFile >> id;
        Node<T> *node = dynamic_cast<Node<T> *>(graph.findVertex(id));

        if (node != nullptr) {
            Node<T> * park;
            int fixedPrice = rand()%2;
            if(fixedPrice){
                park = new FixedPriceParking<T>(*node);
            }
            else{
                park = new DynamicPriceParking<T>(*node);
            }
            dynamic_cast<Node<T> *>(park)->setParking();
            if(!graph.replaceVertex(park)){
                exit(1);
            }
            parkingNodes.push_back(park);
            delete node;
        }
    }
    parkingsFile.close();
                        return parkingNodes;
}
//
//template <class T>
//void readNodesFile(Graph<T> &graph, string nodesfile);
//
//template <class T>
//void readEdgesFile(Graph<T> &graph, bool bidir, string edgesfile);
//
//template <class T>
//vector<Node<T> *> readParkingFile(Graph<T> &graph, string parkingFile);


#endif //PROJECT_FILEMANAGEMENT_H
