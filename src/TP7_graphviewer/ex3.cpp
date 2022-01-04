#include <iostream>
#include <fstream>
#include <sstream>
#include "graphviewer.h"

using namespace std;
using Node = GraphViewer::Node;
using Edge = GraphViewer::Edge;

void ex3() {

    GraphViewer gv;
    gv.setCenter(sf::Vector2f(300, 300));
   // gv.setScale(1.0/4000.0);
    //gv.setCenter(sf::Vector2f(-8.600, -41.146));

    ifstream iNodesStream("../TP7_graphviewer/resources/map1/nodes.txt");
    GraphViewer::id_t N, idNode; double lat, lon;
    iNodesStream >> N;
    for(GraphViewer::id_t i = 0; i < N; ++i){
        iNodesStream >> idNode >> lat >> lon;
        Node &node = gv.addNode(idNode, sf::Vector2f(lon, -lat));

    }

    ifstream iEdgesStream("../TP7_graphviewer/resources/map1/edges.txt");
    GraphViewer::id_t E, idEdge, u, v;
    iEdgesStream >> E;
    for(GraphViewer::id_t i = 0; i < E; ++i){
        iEdgesStream >> idEdge >> u >> v;
        Edge &edge = gv.addEdge(idEdge, gv.getNode(u), gv.getNode(v));

    }

    //gv.setBackground("../TP7_graphviewer/resources/map2/map.jpg");


    // Create window
    gv.createWindow(600, 600);
    // Join viewer thread (blocks till window closed)
    gv.join();
}

