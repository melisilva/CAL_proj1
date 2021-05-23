//
// Created by henri on 12/05/2021.
//

#ifndef PROJECT_EDGE_H
#define PROJECT_EDGE_H


template<class T>
class Vertex;

template<class T>
class Graph;

template<class T>
class Edge {
    Vertex<T> *orig;
    Vertex<T> *dest;
    double capacity;
    double cost;
    double flow;

    Edge(Vertex<T> *o, Vertex<T> *d, double capacity, double cost = 0, double flow = 0);

public:
    friend class Graph<T>;

    friend class Vertex<T>;

    double getFlow() const;

    double getCost() const;

    double getCapacity() const;

    Vertex<T> *getOrig();

    Vertex<T> *getDest();
};

template<class T>
Edge<T>::Edge(Vertex<T> *o, Vertex<T> *d, double capacity, double cost, double flow):
        orig(o), dest(d), capacity(capacity), cost(cost), flow(flow) {}

template<class T>
double Edge<T>::getFlow() const {
    return this->flow;
}

template<class T>
double Edge<T>::getCapacity() const {
    return this->capacity;
}

template<class T>
double Edge<T>::getCost() const {
    return this->cost;
}

template<class T>
Vertex<T> *Edge<T>::getOrig() {
    return this->orig;
}

template<class T>
Vertex<T> *Edge<T>::getDest() {
    return this->dest;
}


#endif //PROJECT_EDGE_H