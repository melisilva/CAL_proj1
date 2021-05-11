#include "FileManagement.h"
template <class T>
void readNodesFile(Graph<T> &graph, string nodesfile){
    //node file
    ifstream nodes;
    string line;
    int numNodes;

    nodes.open(nodesfile);

    getline(nodes, line);
    numNodes = stoi(line);

    for (int i = 0; i < numNodes; i++) {
        getline(nodes, line);
        int id;
        double x, y;
        size_t pos = line.find(',');
        id = stoi(line.substr(1, pos));
        line.erase(0, pos + 1);
        pos = line.find(',');
        x = stof(line.substr(0, pos));
        line.erase(0, pos + 1);
        pos = line.find(')');
        y = stof(line.substr(0, pos));
        graph.addVertex(make_pair(x, y)); //change addVertex to have id?
    }
    nodes.close();
}

template <class T>
void readEdgesFile(Graph<T> &graph, bool bidir, string edgesfile){
    //edges file
    ifstream edges;
    string line;

    edges.open(edgesfile);

    getline(edges, line);
    int numEdges = stoi(line);
    for (int i = 0; i < numEdges; i++) {
        getline(edges, line);
        int n1, n2;
        size_t pos = line.find(',');
        n1 = stoi(line.substr(1, pos));
        line.erase(0, pos + 1);
        pos = line.find(')');
        n2 = stoi(line.substr(0, pos));

        Vertex<T>* v1 = graph.findVertex(n1);
        Vertex<T>* v2 = graph.findVertex(n2);
        double weight = graph.heuristic(v1, v2);
        if(bidir)
            graph.addBiEdge(v1->getId(), v2->getId(), weight); //no idea how to get cost & flow?--->new function?
        else
            graph.addEdge(v1->getId(), v2->getId(), weight); //no idea how to get cost & flow?--->new function?
    }
    edges.close();

}

