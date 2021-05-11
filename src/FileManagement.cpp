#include "FileManagement.h"

//template<class T>
//void readNodesFile(Graph<T> &graph, string nodesfile) {
//    //node file
//    ifstream nodes;
//    string line;
//    int numNodes;
//
//    nodes.open(nodesfile);
//
//    getline(nodes, line);
//    numNodes = stoi(line);
//
//    for (int i = 0; i < numNodes; i++) {
//        getline(nodes, line);
//        int id;
//        double x, y;
//        size_t pos = line.find(',');
//        id = stoi(line.substr(1, pos));
//        line.erase(0, pos + 1);
//        pos = line.find(',');
//        x = stof(line.substr(0, pos));
//        line.erase(0, pos + 1);
//        pos = line.find(')');
//        y = stof(line.substr(0, pos));
//        Node<T> *nodeToAdd = new Node<T>(id, x, y);
//        graph.addVertex(nodeToAdd);
//    }
//    nodes.close();
//}
//
//template<class T>
//void readEdgesFile(Graph<T> &graph, bool bidir, string edgesfile) {
//    //edges file
//    ifstream edges;
//    string line;
//
//    edges.open(edgesfile);
//
//    getline(edges, line);
//    int numEdges = stoi(line);
//    for (int i = 0; i < numEdges; i++) {
//        getline(edges, line);
//        int n1, n2;
//        size_t pos = line.find(',');
//        n1 = stoi(line.substr(1, pos));
//        line.erase(0, pos + 1);
//        pos = line.find(')');
//        n2 = stoi(line.substr(0, pos));
//
//        Vertex<T> *v1 = graph.findVertex(n1);
//        Vertex<T> *v2 = graph.findVertex(n2);
//        double weight = graph.heuristic(v1, v2);
//        if (bidir)
//            graph.addBiEdge(v1->getId(), v2->getId(), weight);
//        else
//            graph.addEdge(v1->getId(), v2->getId(), 0, weight, 0);
//        //capacity and flow aren't used for our problem
//    }
//    edges.close();
//
//}
//
//template<class T>
//vector<Node<T> *> readParkingFile(Graph<int> &graph, string parkingFile) {
//    //parking file
//    ifstream parkingsFile;
//    string line;
//    int noParkings, id;
//    vector<Node<T> *> parkingNodes;
//
//    parkingsFile.open(parkingFile);
//
//    parkingsFile >> noParkings;
//    getline(parkingsFile, line); // amenity=car_parking
//    for (int i = 0; i < noParkings; i++) {
//        parkingsFile >> id;
//        Node<T> *node = graph.findVertex(id);
//        if (node != nullptr) {
//            node->setParking();
//            parkingNodes.push_back(node);
//        }
//    }
//    parkingsFile.close();
//    return parkingNodes;
//}
//
