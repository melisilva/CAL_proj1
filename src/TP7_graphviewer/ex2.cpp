#include "graphviewer.h"

using namespace std;
using Node = GraphViewer::Node;
using Edge = GraphViewer::Edge;

void ex2() {
    // Instantiate GraphViewer
    GraphViewer gv;
    //Set coordinates of window center
    gv.setCenter(sf::Vector2f(300, 300));

    //blue vertex id 0 at (200,300)
    Node &node0 = gv.addNode(0, sf::Vector2f(300, 50));// Create node
    Node &node1 = gv.addNode(1, sf::Vector2f(318, 58));// Create node
    Node &node2 = gv.addNode(2, sf::Vector2f(325, 75));// Create node
    Node &node3 = gv.addNode(3, sf::Vector2f(318, 93));// Create node
    Node &node4 = gv.addNode(4, sf::Vector2f(300, 100));// Create node
    Node &node5 = gv.addNode(5, sf::Vector2f(282, 93));// Create node
    Node &node6 = gv.addNode(6, sf::Vector2f(275, 75));// Create node
    Node &node7 = gv.addNode(7, sf::Vector2f(282, 58));// Create node
    Node &node8 = gv.addNode(8, sf::Vector2f(150, 200));// Create node
    Node &node9 = gv.addNode(9, sf::Vector2f(300, 200));// Create node
    Node &node10 = gv.addNode(10, sf::Vector2f(450, 200));// Create node
    Node &node11 = gv.addNode(11, sf::Vector2f(300, 400));// Create node
    Node &node12 = gv.addNode(12, sf::Vector2f(200, 550));// Create node
    Node &node13 = gv.addNode(13, sf::Vector2f(400, 550));// Create node

    Edge &edge0 = gv.addEdge(0,node0,node1,GraphViewer::Edge::EdgeType::DIRECTED);
    Edge &edge1 = gv.addEdge(1,node1,node2,GraphViewer::Edge::EdgeType::DIRECTED);
    Edge &edge2 = gv.addEdge(2,node2,node3,GraphViewer::Edge::EdgeType::DIRECTED);
    Edge &edge3 = gv.addEdge(3,node3,node4,GraphViewer::Edge::EdgeType::DIRECTED);
    Edge &edge4 = gv.addEdge(4,node4,node5,GraphViewer::Edge::EdgeType::DIRECTED);
    Edge &edge5 = gv.addEdge(5,node5,node6,GraphViewer::Edge::EdgeType::DIRECTED);
    Edge &edge6 = gv.addEdge(6,node6,node7,GraphViewer::Edge::EdgeType::DIRECTED);
    Edge &edge7 = gv.addEdge(7,node7,node0,GraphViewer::Edge::EdgeType::DIRECTED);
    Edge &edge8 = gv.addEdge(8,node4,node9,GraphViewer::Edge::EdgeType::DIRECTED);
    Edge &edge9 = gv.addEdge(9,node9,node8,GraphViewer::Edge::EdgeType::DIRECTED);
    Edge &edge10 = gv.addEdge(10,node9,node10,GraphViewer::Edge::EdgeType::DIRECTED);
    Edge &edge11 = gv.addEdge(11,node9,node11,GraphViewer::Edge::EdgeType::DIRECTED);
    Edge &edge12= gv.addEdge(12,node11,node12,GraphViewer::Edge::EdgeType::DIRECTED);
    Edge &edge13 = gv.addEdge(13,node11,node13,GraphViewer::Edge::EdgeType::DIRECTED);

    Node &node14 = gv.getNode(12);// Get reference to node
    node14.setPosition(sf::Vector2f(250, 550));// Set position

    Node &node15 = gv.getNode(13);// Get reference to node
    node15.setPosition(sf::Vector2f(350, 550));// Set position

    gv.setBackground("../TP7_graphviewer/resources/background.png");

    // Create window
    gv.createWindow(600, 600);
    // Join viewer thread (blocks till window closed)
    gv.join();
}

