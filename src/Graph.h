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
#include <unordered_map>
#include "Edge.h"
#include "Path.h"
#include "MutablePriorityQueue.h"

using namespace std;

constexpr auto INF = std::numeric_limits<double>::max();

template<class T>
class Vertex;

template<class T>
class Graph;

template<class T>
class Graph {
    vector<Vertex<T> *> vertexSet;

    void dfsVisit(Vertex<T> *v, std::vector<T> &res) const;

    void dijkstraShortestPath(Vertex<T> *s);

    void bellmanFordShortestPath(Vertex<T> *s);

    bool relax(Vertex<T> *v, Vertex<T> *w, Edge<T> *e, double residual, double cost);

    void resetFlows();

    bool findAugmentationPath(Vertex<T> *s, Vertex<T> *t);

    void testAndVisit(queue<Vertex<T> *> &q, Edge<T> *e, Vertex<T> *w, double residual);

    double findMinResidualAlongPath(Vertex<T> *s, Vertex<T> *t);

    void augmentFlowAlongPath(Vertex<T> *s, Vertex<T> *t, double flow);

    int **P = nullptr; //path

public:
    Vertex<T> *findVertex(const T &inf) const;

    int findVertexIdx(const T &id) const;

    vector<Vertex<T> *> getVertexSet() const;

    Vertex<T> *addVertex(const T &in);

    Vertex<T> *addVertex(Vertex<T> *v);

    bool replaceVertex(Vertex<T> *v);

    bool addBiEdge(const T &sourc, const T &dest, double w);

    Edge<T> *addEdge(const T &sourc, const T &dest, double capacity, double cost, double flow = 0);

    double getFlow(const T &sourc, const T &dest) const;

    void fordFulkerson(T source, T target);

    double minCostFlow(T source, T target, double flow);

    double aStar(Graph<T> graph, Vertex<T> *p, Vertex<T> *d);

    int getPath(const int &orig, const int &dest) const;

    void initializeForShortestPath();

    void initializeForShortestPathwalking();

    double distancePath(queue<Vertex<T> *> path);

    double heuristic(Vertex<T> *v1, Vertex<T> *v2);

    queue<Vertex<T> *> dijkstraShortestPath(const int &origin, const int &dest);

    Path *getNextClosestParking(Node<int> *v, bool reset = false);

    Path *aStarShortestPath(const int &origin, const int &dest);

    Path *aStarShortestPathwalking(const int &origin, const int &dest);

    vector<T> dfs() const;

    vector<T> bfs(Node<int> *source);
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
bool Graph<T>::replaceVertex(Vertex<T> *v) {
    int id = findVertexIdx(v->info);
    if (id == -1) {
        return false;
    }
    vertexSet[id] = v;
    return true;
}

template<class T>
int Graph<T>::findVertexIdx(const T &id) const {
    for (int i = 0; i < vertexSet.size(); i++) {
        if (vertexSet[i]->info == id) {
            return i;
        }
    }
    return -1;
}

template<class T>
vector<Vertex<T> *> Graph<T>::getVertexSet() const {
    return vertexSet;
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
        v->pathV = NULL;
        v->queueIndex = 0;
    }
}

template<class T>
void Graph<T>::initializeForShortestPathwalking() {
    for (Vertex<T> *v: vertexSet) {
        v->dist = INT_MAX;
        v->distI = INT_MAX;
        v->queueIndex = 0;
        v->queueIndexI = 0;
        v->pathV = NULL;
        v->pathI = NULL;
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
Path *Graph<T>::getNextClosestParking(Node<int> *node, bool reset) {
    static MutablePriorityQueue<Vertex<T>> q;
    static Vertex<T> *final;
    queue<Vertex<T> *> path;
    Path *nodePath;
    cout << "hloololo" << endl;
    //Node<T> *node = dynamic_cast<Node<T> *>(v);
    cout << "still here" << endl;


    if (node->getParking()) {
        nodePath->appendPath(node);
        return nodePath;
    }

    if (reset) {
        initializeForShortestPath();
        q = MutablePriorityQueue<Vertex<T>>();
        node->dist = 0;
        q.insert(node);
    }

    while (!q.empty()) {
        Vertex<T> *temp = q.extractMin();

        if (dynamic_cast<Node<T> *>(temp)->getParking()) {
            final = temp;
            break;
        }

        for (Edge<T> *edge: temp->getOutgoing()) {
            Vertex<T> *v = edge->getDest();
            bool notFound = (v->getDist() == INT_MAX);

            if (temp->getDist() + edge->cost < v->getDist()) {
                // SHOULD TEST
                v->dist = temp->getDist() + edge->cost;
                v->path = edge;
                v->pathV = edge->getOrig();
                if (notFound) q.insert(v);
                else q.decreaseKey(v);
            }

        }

    }

    path.push(final);
    Vertex<T> *previous = final->getPath();
    path.push(previous);
    nodePath = new Path(dynamic_cast<Node<int> *>(final));
    while (previous != node) {
        previous = previous->getPath();
        nodePath->appendPath(dynamic_cast<Node<T> *>(previous));
    }
    nodePath->setLength(final->dist);
    return nodePath; //Path from the park to the node given

}

template<class T>
Path *Graph<T>::aStarShortestPath(const int &origin, const int &dest) {

    Path *path;
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

        for (Edge<T> *edge: temp->getOutgoing()) {

            Vertex<T> *v = edge->getDest();

            double f = temp->getDist() - heuristic(temp, final) + heuristic(v, final) + edge->getCost();

            bool notFound = (v->getDist() == INT_MAX);

            if (f < v->getDist()) {
                v->dist = f;
                v->pathV = temp;

                if (notFound) q.insert(v);
                else q.decreaseKey(v);
            }
        }
    }


    path->appendPath(dynamic_cast<Node<T> *>(final));

    Vertex<T> *previous = final->getPath();
    path->appendPath(dynamic_cast<Node<T> *>(previous));


    while (previous != orig) {
        previous = previous->getPath();
        path->appendPath(dynamic_cast<Node<T> *>(previous));
    }

    return path;
}


template<class T>
Path *Graph<T>::aStarShortestPathwalking(const int &origin, const int &dest) {

    Path *path;
    if (origin == dest) return path;

    initializeForShortestPathwalking();

    //Maps to keep track of visited vertices
    unordered_map<int, bool> fVisited;
    unordered_map<int, bool> iVisited;

    //Creating forward and reverse priority queues
    MutablePriorityQueue<Vertex<T>> fQueue;
    MutablePriorityQueue<Vertex<T>> iQueue(true);


    //Starting values
    Vertex<T> *src = findVertex(origin), *final = findVertex(dest), *mid;
    src->dist = 0;
    final->distI = 0;

    fQueue.insert(src);
    iQueue.insert(final);

    while (!fQueue.empty() && !iQueue.empty()) {
        //Forward search
        //Starting by extracting the minimum from forward queue and set vertex as visited by forward search
        Vertex<T> *fV = fQueue.extractMin();
        fVisited.emplace(fV->getInfo(), true);

        //Checks if it has been visited by reverse search, if so the path has been found
        if (iVisited.count(fV->getInfo()) > 0) {
            mid = fV;
            break;
        }

        //Explore edges in regular graph
        for (Edge<T> *e: fV->outgoing) {
            Vertex<T> *v = e->getDest();
            double temp = fV->getDist() + e->getCost() - heuristic(fV, final) + heuristic(v, final);
            bool notFound = (v->getDist() == INT_MAX);
            if (v->getDist() > temp) {
                v->dist = temp;
                v->pathV = fV;
                if (notFound) fQueue.insert(v);
                else
                    fQueue.decreaseKey(v);
            }
        }

        //Reverse seacrh
        //Starting by extracting the minimum from reverse queue and set vertex as visited by reverse search
        Vertex<T> *iV = iQueue.extractMin();
        iVisited.emplace(iV->getInfo(), true);

        //Checks if it has been visited by forward search, if so the path has been found
        if (fVisited.count(iV->getInfo()) > 0) {
            mid = iV;
            break;
        }


        //Explore edges in reverse graph
        for (Edge<T> *e: iV->incoming) {
            Vertex<T> *v = e->getOrig();

            double temp = iV->distI + e->getCost() - heuristic(iV, src) + heuristic(v, src);
            bool notFound = (v->distI == INT_MAX);
            if (v->distI > temp) {
                v->distI = temp;
                v->pathI = iV;
                if (notFound) iQueue.insert(v);
                else
                    iQueue.decreaseKey(v);
            }
        }
    }

    //Verify if alternatives paths are better, not passing through mid vertex
    queue<Vertex<T> *> aux;
    double f = mid->dist - heuristic(mid, final) + heuristic(mid, src) + mid->distI;

    for (auto node : fVisited) {
        Vertex<T> *temp = findVertex(node.first);

        for (Edge<T> *e: temp->outgoing) {

            Vertex<T> *tent = e->getDest();
            if (iVisited.count(tent->getInfo()) > 0) {

                double f_temp = temp->getDist() + tent->getDist() + e->getCost() - heuristic(temp, final) - heuristic(tent, src);

                if (f_temp < f) {
                    tent->pathV = temp;
                    mid = tent;
                    f = f_temp;
                }

            }
        }
    }

    //Start to build the final path from mid
    vector<Vertex<T> *> t;
    t.push_back(mid);
    Vertex<T> *v = mid;

    while (v->path != NULL) {
        v = v->pathV;
        t.emplace(t.begin(), v);
    }

    v = mid;

    while (v->pathI != NULL) {
        v = v->pathI;
        t.push_back(v);
    }

    for (unsigned i = t.size(); i-- > 0;) {
        path->appendPath(dynamic_cast<Node<T> *>(t.at(i)));
    }

    return path;
}


/*Analyze graph connectivity:
 * There's two ways:
 * 1-Using dfs->goes through all the graph's vertexes
 * 2-Using bfs->goes through only the graph's vertexes that it's possible to reach from the origin vertex */

template<class T>
std::vector<T> Graph<T>::dfs() const {
    std::vector<T> res;
    for (auto v : vertexSet)
        v->visited = false;
    for (auto v : vertexSet)
        if (!v->visited)
            dfsVisit(v, res);
    return res;
}

template<class T>
void Graph<T>::dfsVisit(Vertex<T> *v, std::vector<T> &res) const {
    v->visited = true;
    res.push_back(v->info);
    for (auto &e : v->outgoing) {
        auto w = e->dest;
        if (!w->visited)
            dfsVisit(w, res);
    }
}

template<class T>
std::vector<T> Graph<T>::bfs(Node<int> *source) {
    std::vector<T> res;
    auto s = findVertex(source->getInfo());
    if (s == NULL)
        return res;
    std::queue<Vertex<T> *> q;
    for (auto v : vertexSet)
        v->visited = false;
    q.push(s);
    s->visited = true;
    while (!q.empty()) {
        auto v = q.front();
        q.pop();
        res.push_back(v->info);
        for (auto &e : v->outgoing) {
            auto w = e->dest;
            if (!w->visited) {
                q.push(w);
                w->visited = true;
            }
        }
    }
    return res;
}


#endif /* GRAPH_H_ */

