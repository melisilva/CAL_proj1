/*
 * Graph.h.
 * For implementation of the minimum cost flow algorithm.
 * See TODOs for code to add/adapt.
 * FEUP, CAL, 2017/18.
 */
#ifndef GRAPH_H_
#define GRAPH_H_

#include <vector>
#include <queue>
#include <limits>
#include <iostream>
#include <string>
#include <sstream>
#include <cmath>
#include <unordered_map>
#include "MutablePriorityQueue.h"

using namespace std;

constexpr auto INF = std::numeric_limits<double>::max();

template<class T>
class Vertex;

template<class T>
class Edge;

template<class T>
class Graph;

/*
 * ================================================================================================
 * Class Vertex
 * ================================================================================================
 */

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
vector<Edge<T> *> Vertex<T>::getIncoming() const {
    return this->incoming;
}

template<class T>
vector<Edge<T> *> Vertex<T>::getOutgoing() const {
    return this->outgoing;
}

template<class T>
Vertex<T> *Vertex<T>::getPath() const {
    return this->path;
}

/* ================================================================================================
 * Class Edge
 * ================================================================================================
 */

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
Vertex<T> *Edge<T>::getOrig() {
    return this->orig;
}

template<class T>
Vertex<T> *Edge<T>::getDest() {
    return this->dest;
}


/* ================================================================================================
 * Class Graph
 * ================================================================================================
 */

template<class T>
class Graph {
    vector<Vertex<T> *> vertexSet;


    void dijkstraShortestPath(Vertex<T> *s);

    void bellmanFordShortestPath(Vertex<T> *s);

    bool relax(Vertex<T> *v, Vertex<T> *w, Edge<T> *e, double residual, double cost);

    void resetFlows();

    bool findAugmentationPath(Vertex<T> *s, Vertex<T> *t);

    void testAndVisit(queue<Vertex<T> *> &q, Edge<T> *e, Vertex<T> *w, double residual);

    double findMinResidualAlongPath(Vertex<T> *s, Vertex<T> *t);

    void augmentFlowAlongPath(Vertex<T> *s, Vertex<T> *t, double flow);

    int **P = nullptr; //path
    vector<int> currentPoints;

public:
    Vertex<T> *findVertex(const T &inf) const;

    int findVertexIdx(const int &id) const;

    vector<Vertex<T> *> getVertexSet() const;

    Vertex<T> *addVertex(const T &in);

    Vertex<T> *addVertex(Vertex<T> *v);

    bool addBiEdge(const T &sourc, const T &dest, double w);

    Edge<T> *addEdge(const T &sourc, const T &dest, double capacity, double cost, double flow = 0);

    double getFlow(const T &sourc, const T &dest) const;

    void fordFulkerson(T source, T target);

    double minCostFlow(T source, T target, double flow);

    double aStar(Graph<T> graph, Vertex<T> *p, Vertex<T> *d);

    int getPath(const int &orig, const int &dest) const;

    void initializeForShortestPath();

    double distancePath(queue<Vertex<T> *> path);

    double heuristic(Vertex<T> *v1, Vertex<T> *v2);

    queue<Vertex<T> *> dijkstraShortestPath(const int &origin, const int &dest);

    queue<Vertex<T> *> aStarShortestPath(const int &origin, const int &dest);
};

template<class T>
Vertex<T> *Graph<T>::addVertex(const T &in) {
    Vertex<T> *v = findVertex(in);
    if (v != nullptr)
        return v;
    v = new Vertex<T>(in);
    vertexSet.push_back(v);
    return v;
}

template<class T>
Vertex<T> *Graph<T>::addVertex(Vertex<T> *v) {
    vertexSet.push_back(v);
    return v;
}


template<class T>
int Graph<T>::findVertexIdx(const int &id) const {
    for (unsigned i = 0; i < currentPoints.size(); i++)
        if (currentPoints.at(i) == id)
            return i;
    return -1;
}

template<class T>
int Graph<T>::getPath(const int &orig, const int &dest) const {
    int i = findVertexIdx(orig);
    int j = findVertexIdx(dest);
    return P[i][j];
}

template<class T>
Edge<T> *Graph<T>::addEdge(const T &sourc, const T &dest, double capacity, double cost, double flow) {
    auto s = findVertex(sourc);
    auto d = findVertex(dest);
    if (s == nullptr || d == nullptr)
        return nullptr;
    Edge<T> *e = new Edge<T>(s, d, capacity, cost, flow);
    s->addEdge(e);
    return e;
}

template<class T>
bool Graph<T>::addBiEdge(const T &sourc, const T &dest, double w) {
    auto v1 = findVertex(sourc);
    auto v2 = findVertex(dest);

    if (v1 == nullptr || v2 == nullptr)
        return false;

    bool found = false;
    for (Edge<T> *e: v1->getOutgoing()) {
        if (e->dest->getInfo() == dest) {
            found = true;
            break;
        }
    }

    if (!found) {
        v1->addEdge(new Edge<T>(v1, v2, 0, w, 0));
    }


    for (Edge<T> *e: v2->getOutgoing()) {
        if (e->dest->getInfo() == sourc)
            return true;
    }

    v2->addEdge(new Edge<T>(v2, v1, 0, w, 0));

    return true;
}


template<class T>
Vertex<T> *Graph<T>::findVertex(const T &inf) const {
    for (auto v : vertexSet)
        if (v->info == inf)
            return v;
    return nullptr;
}

template<class T>
double Graph<T>::getFlow(const T &sourc, const T &dest) const {
    auto s = findVertex(sourc);
    auto d = findVertex(dest);
    if (s == nullptr || d == nullptr)
        return 0.0;
    for (auto e : s->outgoing)
        if (e->dest == d)
            return e->flow;
    return 0.0;
}

template<class T>
vector<Vertex<T> *> Graph<T>::getVertexSet() const {
    return vertexSet;
}

template<class T>
double Graph<T>::distancePath(queue<Vertex<T> *> path) {
    if (path.empty())
        return 0;

    double res = 0;
    Vertex<T> *v1 = path.front();
    path.pop();
    Vertex<T> *v2;

    while (!path.empty()) {
        v2 = path.front();
        path.pop();
        for (Edge<T> *e: v2->getAdj()) {
            if (e->getDest() == v1) {
                res += e->getWeight();
                break;
            }
        }
        v1 = v2;
    }
    return res / 1000;
}


/**************** Maximum Flow Problem  ************/

/**
 * Finds the maximum flow in a graph using the Ford Fulkerson algorithm
 * (with the improvement of Edmonds-Karp).
 * Assumes that the graph forms a flow network from source vertex 's'
 * to sink vertex 't' (distinct vertices).
 * Receives as arguments the source and target vertices (identified by their contents).
 * The result is defined by the "flow" field of each edge.
 */
template<class T>
void Graph<T>::fordFulkerson(T source, T target) {
    // Obtain the source (s) and target (t) vertices
    Vertex<T> *s = findVertex(source);
    Vertex<T> *t = findVertex(target);
    if (s == nullptr || t == nullptr || s == t)
        throw "Invalid source and/or target vertex";

    // Apply algorithm as in slides
    resetFlows();
    while (findAugmentationPath(s, t)) {
        double f = findMinResidualAlongPath(s, t);
        augmentFlowAlongPath(s, t, f);
    }
}

template<class T>
void Graph<T>::resetFlows() {
    for (auto v : vertexSet)
        for (auto e: v->outgoing)
            e->flow = 0;
}

template<class T>
bool Graph<T>::findAugmentationPath(Vertex<T> *s, Vertex<T> *t) {
    for (auto v : vertexSet)
        v->visited = false;
    s->visited = true;
    queue<Vertex<T> *> q;
    q.push(s);
    while (!q.empty() && !t->visited) {
        auto v = q.front();
        q.pop();
        for (auto e: v->outgoing)
            testAndVisit(q, e, e->dest, e->capacity - e->flow);
        for (auto e: v->incoming)
            testAndVisit(q, e, e->orig, e->flow);
    }
    return t->visited;
}

/**
 * Auxiliary function used by findAugmentationPath.
 */
template<class T>
void Graph<T>::testAndVisit(queue<Vertex<T> *> &q, Edge<T> *e, Vertex<T> *w, double residual) {
    // TODO: adapt in order to use only edges with null cost
    if (!w->visited && residual > 0) {
        w->visited = true;
        w->path = e;
        q.push(w);
    }
}

template<class T>
double Graph<T>::findMinResidualAlongPath(Vertex<T> *s, Vertex<T> *t) {
    double f = INF;
    for (auto v = t; v != s;) {
        auto e = v->path;
        if (e->dest == v) {
            f = min(f, e->capacity - e->flow);
            v = e->orig;
        } else {
            f = min(f, e->flow);
            v = e->dest;
        }
    }
    return f;
}

template<class T>
void Graph<T>::augmentFlowAlongPath(Vertex<T> *s, Vertex<T> *t, double f) {
    for (auto v = t; v != s;) {
        auto e = v->path;
        if (e->dest == v) {
            e->flow += f;
            v = e->orig;
        } else {
            e->flow -= f;
            v = e->dest;
        }
    }
}


/**************** Minimum Cost Flow Problem  ************/


/**
 * Computes the shortest distance (with minimum cost) from "s" to all other vertices
 * in the residuals graph, using only edges with non-null residuals,
 * based on the Dijkstra algorithm.
 * The result is indicated by the field "dist" of each vertex.
 */
template<class T>
void Graph<T>::dijkstraShortestPath(Vertex<T> *s) {
    for (auto v : vertexSet)
        v->dist = INF;
    s->dist = 0;
    MutablePriorityQueue<Vertex<T>> q;
    q.insert(s);
    while (!q.empty()) {
        auto v = q.extractMin();
        for (auto e : v->outgoing) {
            auto oldDist = e->dest->dist;
            if (relax(v, e->dest, e, e->capacity - e->flow, e->cost)) {
                if (oldDist == INF)
                    q.insert(e->dest);
                else
                    q.decreaseKey(e->dest);
            }
        }
        for (auto e : v->incoming) {
            auto oldDist = e->orig->dist;
            if (relax(v, e->orig, e, e->flow, -e->cost)) {
                if (oldDist == INF)
                    q.insert(e->orig);
                else
                    q.decreaseKey(e->orig);
            }
        }
    }
}

/**
 * Computes the shortest distance (with minimum cost) from "s" to all other vertices
 * in the residuals graph, using only edges with non-null residuals,
 * based on the Bellman-Ford algorithm.
 * The result is indicated by the field "dist" of each vertex.
 */
template<class T>
void Graph<T>::bellmanFordShortestPath(Vertex<T> *s) {
    for (auto v : vertexSet)
        v->dist = INF;
    s->dist = 0;
    for (unsigned i = 1; i < vertexSet.size(); i++)
        for (auto v: vertexSet) {
            for (auto e : v->outgoing)
                relax(v, e->dest, e, e->capacity - e->flow, e->cost);
            for (auto e : v->incoming)
                relax(v, e->orig, e, e->flow, -e->cost);
        }
}

/**
 * Auxiliary function used by Dijkstra and Bellman-Ford algorithms.
 * Analyzes edge (v, w) with a given residual and cost.
 */

template<class T>
bool Graph<T>::relax(Vertex<T> *v, Vertex<T> *w, Edge<T> *e, double residual, double cost) {
    if (residual > 0 && v->dist + cost < w->dist) {
        w->dist = v->dist + cost;
        w->path = e;
        w->pathV = v;
        return true;
    } else
        return false;
}

/**
 * Determines the minimum cost flow in a flow network.
 * Receives as arguments the source and sink vertices (identified by their info),
 * and the intended flow.
 * Returns the calculated minimum cost for delivering the intended flow (or the highest
 * possible flow, if the intended flow is higher than supported by the network).
 * The calculated flow in each edge can be consulted with the "getFlow" function.
 * Notice: Currently, the costs of the edges are modified by the algorithm.
 */
template<class T>
double Graph<T>::minCostFlow(T source, T sink, double flow) {
    // TODO: implement based on slides and the given implementation of Ford-Fulkerson algorithm
    Vertex<T> *vSource = findVertex(source);
    Vertex<T> *vSink = findVertex(sink);
    double cost = 0;
    double dt = 0;

    //Reset the flows
    for (auto v: vertexSet) {
        for (auto e: v->outgoing) {
            e->flow = 0;
        }
        for (auto e: v->incoming) {
            e->flow = 0;
        }
    }

    //1 Determine the distance from s to each vertex
    bellmanFordShortestPath(vSource);

    //2 Replace the initial costs by the reduced costs
    for (auto v: vertexSet) {
        for (auto e: v->outgoing) {
            e->cost = e->cost + e->orig->dist - e->dest->dist;
        }
    }

    //3 Selecionar um caminho de custo minimo de s para t no grafo de residuos
    double increment = findMinResidualAlongPath(vSource, vSink);
    if (increment > flow) {
        increment = flow;
    }
    dt += vSink->dist;
    cost += dt * increment;

    //4 Apply the augmentation path
    augmentFlowAlongPath(vSource, vSink, increment);
    flow -= increment;

    //6 Repeat the process
    while (flow > 0) {
        //5 new reduction on the residue graph
        dijkstraShortestPath(vSource);

        for (auto v: vertexSet) {
            for (auto e: v->outgoing) {
                e->cost = e->cost + e->orig->dist - e->dest->dist;
            }
        }
        increment = findMinResidualAlongPath(vSource, vSink);
        if (increment == 0) {
            //All possible paths have been exhausted
            break;
        }
        if (increment > flow) {
            increment = flow;
        }
        dt += vSink->dist;
        cost += dt * increment;
        augmentFlowAlongPath(vSource, vSink, increment);
        flow -= increment;
    }

    return cost;
}

template<class T>
double Graph<T>::heuristic(Vertex<T> *v1, Vertex<T> *v2) {
    return v1->getHeuristic(v2);
}


template<class T>
double Graph<T>::aStar(Graph<T> graph, Vertex<T> *p, Vertex<T> *d) {
    std::unordered_map<Vertex<T> *, Vertex<T> *> cameFrom;
    std::unordered_map<Vertex<T> *, Vertex<T> *> costSoFar;

    MutablePriorityQueue<Vertex<T>> frontier;
    p->dist = 0;
    frontier.push(p);

    cameFrom[p] = p;
    costSoFar[p] = 0;

    while (!frontier.empty()) {
        T current = frontier.pop();

        if (current == d) {
            break;
        }

        for (auto v : graph.getVertexSet()) {
            for (auto next : v->adj) {
                double newCost = costSoFar[current] + next->cost;

                if (costSoFar.find(next) == costSoFar.end() || newCost < costSoFar[next]) {
                    costSoFar[next] = newCost;
                    double priority = newCost; // + heuristic(next, goal)
                    next->cost = priority;
                    frontier.push(next);
                    cameFrom[next] = current;
                }
            }
        }
    }
    return 0;
}


template<class T>
void Graph<T>::initializeForShortestPath() {
    for (Vertex<T> *v: vertexSet) {
        v->dist = INT_MAX;
        v->path = NULL;
        v->queueIndex = 0;
    }
}


template<class T>
queue<Vertex<T> *> Graph<T>::dijkstraShortestPath(const int &origin, const int &dest) {

    queue<Vertex<T> *> path;
    if (origin == dest) return path;

    initializeForShortestPath();
    Vertex<T> *orig = findVertex(origin);
    Vertex<T> *final = findVertex(dest);

    orig->dist = 0;
    MutablePriorityQueue<Vertex<T>> q;
    q.insert(orig);


    while (!q.empty()) {
        Vertex<T> *temp = q.extractMin();

        if (temp == final) break;

        for (Edge<T> *edge: temp->getAdj()) {
            Vertex<T> *v = edge->getDest();
            bool notFound = (v->getDist() == INT_MAX);

            if (relax(temp, v, edge->getWeight())) {
                if (notFound) q.insert(v);
                else q.decreaseKey(v);
            }
        }
    }

    path.push(final);
    Vertex<T> *previous = final->getPath();
    path.push(previous);
    while (previous != orig) {
        previous = previous->getPath();
        path.push(previous);
    }

    return path;
}

template<class T>
queue<Vertex<T> *> Graph<T>::aStarShortestPath(const int &origin, const int &dest) {

    queue<Vertex<T> *> path;
    if (origin == dest) return path;

    initializeForShortestPath();
    Vertex<T> *orig = findVertex(origin);
    Vertex<T> *final = findVertex(dest);

    orig->dist = 0;
    MutablePriorityQueue<Vertex<T>> q;
    q.insert(orig);


    while (!q.empty()) {
        Vertex<T> *temp = q.extractMin();

        if (temp == final) break;

        for (Edge<T> *edge: temp->getAdj()) {

            Vertex<T> *v = edge->getDest();

            double f = temp->getDist() - heuristic(temp, final) + heuristic(v, final) + edge->getWeight();

            bool notFound = (v->getDist() == INT_MAX);

            if (f < v->getDist()) {
                v->dist = f;
                v->path = temp;

                if (notFound) q.insert(v);
                else q.decreaseKey(v);
            }
        }
    }


    path.push(final);
    Vertex<T> *previous = final->getPath();
    path.push(previous);
    while (previous != orig) {
        previous = previous->getPath();
        path.push(previous);
    }

    return path;
}

template<class T>
class Node : public Vertex<T> {

    double lat, longi;
    bool isParking = false;
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
};


#endif /* GRAPH_H_ */
