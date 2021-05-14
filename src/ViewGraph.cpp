//
// Created by Melissa Silva on 14/05/2021.
//

#include "ViewGraph.h"
#include "graphviewer.h"
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;
using Node = GraphViewer::Node;
using Edge = GraphViewer::Edge;

void testMeli() {
    // Instantiate GraphViewer
    GraphViewer gv;
    //Set coordinates of window center
    gv.setCenter(sf::Vector2f(300, 300));

    //blue vertex id 0 at (200,300)
    Node &node0 = gv.addNode(0, sf::Vector2f(200, 300));// Create node
    node0.setColor(GraphViewer::BLUE); // Change color
    //blue vertex id 1 at (400,300)
    Node &node1 = gv.addNode(1, sf::Vector2f(400, 300));// Create node
    node0.setColor(GraphViewer::BLUE); // Change color

    // for bidirectional edges
    Edge &edge0 = gv.addEdge(0,node0,node1,GraphViewer::Edge::EdgeType::DIRECTED);
    //for directed edgesEdge
    // &edge1 =gv.addEdge(idEdge,idSource,idDestination,GraphViewer::Edge::EdgeType::DIRECTED);

    //Remove vertex 1
    gv.removeNode(1);

    //Add new vertex with ID 2 at (500,300)
    Node &node2 = gv.addNode(2, sf::Vector2f(500, 300));// Create node
    Edge &edge1 = gv.addEdge(1,node0,node2,GraphViewer::Edge::EdgeType::DIRECTED);
    node2.setLabel("This is a vertex");

    edge1.setLabel("This is an edge");

    node2.setColor(GraphViewer::GREEN);

    for(Edge *edge: gv.getEdges())edge->setColor(GraphViewer::YELLOW);

    //gv.setBackground("../TP7_graphviewer/resources/background.png");

    // Create window
    gv.createWindow(600, 600);
    // Join viewer thread (blocks till window closed)
    gv.join();
}
