//
// Created by Melissa Silva on 14/05/2021.
//

#include "ViewGraph.h"
#include "graphviewer.h"
#include "Path.h"
#include "Graph.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;
using N = GraphViewer::Node;
using E = GraphViewer::Edge;

void testMeli(Graph<int>graph, vector<Node<int> *> parkingNodes) {
    // Instantiate GraphViewer
    GraphViewer gv;
    //Set coordinates of window center
    //gv->setCenter(sf::Vector2f(450, 450));
    float min_lon = -8.6889783;
    float min_lat=41.1385607;
    float max_lon=-8.5545839;
    float max_lat=41.1858236;


    for (Vertex<int> *vertex: graph.getVertexSet()) { //graph's nodes/vertexes
        Node<int> *node = dynamic_cast<Node<int> *>(vertex);
        N &vertex_node = gv.addNode(node->getInfo(),
                                     sf::Vector2f((node->getLongitude()-min_lon)/(max_lon-min_lon)*900,(node->getLatitude()-min_lat)/(max_lat-min_lat)*900)); //add nodes from path


        /*if (find(parkingNodes.begin(), parkingNodes.end(), vertex_node) != parkingNodes.end()) {
            vertex_node.setColor(GraphViewer::BLUE); //if node is a park, it will be colored blue
        } else {
            vertex_node.setColor(GraphViewer::GREEN); //if node isn't a park, it will be colored green
        }*/
        vertex_node.setColor(GraphViewer::GREEN); //if node isn't a park, it will be colored green

    }

    for (Node<int> *node:parkingNodes) {
        N &path_node = gv.addNode(node->getInfo(),
                                   sf::Vector2f(node->getLongitude(), node->getLatitude())); //add nodes from path
        path_node.setColor(GraphViewer::ORANGE);
    }


        gv.setBackground("../Mapa da cidade do Porto-20210505/porto_full.png");

        // Create window
        gv.createWindow(900, 900);
        // Join viewer thread (blocks till window closed)
        gv.join();


}

void showPath(Path path, GraphViewer*gv) {
    /*for (Node<int> *node:path.getAllNodes()) {
        N &path_node = gv->addNode(node->getInfo(),
                                   sf::Vector2f(node->getLatitude(), node->getLongitude())); //add nodes from path
        path_node.setColor(GraphViewer::MAGENTA);
    }*/

   /* for(int i=0;i<path.getAllNodes().size();i++){
        gv->addEdge(i,)
    }*/
}