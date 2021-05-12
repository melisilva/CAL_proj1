//
// Created by henri on 12/05/2021.
//

#ifndef PROJECT_VERTEX_H
#define PROJECT_VERTEX_H

#include <vector>
#include "MutablePriorityQueue.h"

using namespace std;

template<class T>
class Edge;

template<class T>
class Graph;


template<class T>
class Vertex {
protected:
    T info;
    vector<Edge<T> *> outgoing;
    vector<Edge<T> *> incoming;

    bool visited;  // for path finding
    Edge<T> *path; // for path finding
    double dist;   // for path finding
    int queueIndex = 0; // required by MutablePriorityQueue

    Vertex<T> *pathV = nullptr;
    Vertex<T> *pathI = nullptr;

public:
    Vertex(T in);

    void addEdge(Edge<T> *e);

    bool operator<(Vertex<T> &vertex) const; // required by MutablePriorityQueue
    T getInfo() const;

    double getDist() const;

    vector<Edge<T> *> getIncoming() const;

    vector<Edge<T> *> getOutgoing() const;

    friend class Graph<T>;

    friend class MutablePriorityQueue<Vertex<T>>;

    double virtual getHeuristic(Vertex<T> *v) { return 0; };

    Vertex *getPath() const;
};


template<class T>
Vertex<T>::Vertex(T in): info(in) {}

template<class T>
void Vertex<T>::addEdge(Edge<T> *e) {
    outgoing.push_back(e);
    e->dest->incoming.push_back(e);
}

template<class T>
bool Vertex<T>::operator<(Vertex<T> &vertex) const {
    return this->dist < vertex.dist;
}

template<class T>
T Vertex<T>::getInfo() const {
    return this->info;
}

template<class T>
double Vertex<T>::getDist() const {
    return dist;
}

template<class T>
vector<Edge<T> *> Vertex<T>::getIncoming() const {
    return this->incoming;
}

template<class T>
vector<Edge<T> *> Vertex<T>::getOutgoing() const {
    return this->outgoing;
}

template<class T>
Vertex<T> *Vertex<T>::getPath() const {
    return this->pathV;
}


#endif //PROJECT_VERTEX_H
