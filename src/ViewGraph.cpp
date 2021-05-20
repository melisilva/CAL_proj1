//
// Created by Melissa Silva on 14/05/2021.
//

#include "ViewGraph.h"
#include "graphviewer.h"
#include "Path.h"
#include "Graph.h"
#include "Parking.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;
using N = GraphViewer::Node;
using E = GraphViewer::Edge;

void testMeli(Graph<int>graph, vector<Node<int> *> parkingNodes) {
    // Instantiate GraphViewer
    int counter=0;
    int edge_id=0;
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
                                    sf::Vector2f((node->getLongitude() - min_lon) / (max_lon - min_lon) * 900,
                                                 (node->getLatitude() - min_lat) / (max_lat - min_lat) *
                                                 900)); //add nodes from path



        if(!node->getParking()){
            counter++;
            vertex_node.setColor(GraphViewer::BLUE); //if node is a park, it will be colored blue
        }
        else{
            vertex_node.setColor(GraphViewer::GREEN); //if node isn't a park, it will be colored green
        }
    }

    for (Vertex<int> *vertex: graph.getVertexSet()) {
        for(Edge<int> *edge:vertex->getOutgoing()){
            E  &edge0 = gv.addEdge(edge_id,gv.getNode(edge->getOrig()->getInfo()),gv.getNode(edge->getDest()->getInfo()),GraphViewer::Edge::EdgeType::DIRECTED);
            edge0.setColor(GraphViewer::RED);
            edge_id++;
        }

        for(Edge<int> *edge:vertex->getIncoming()){
            E  &edge1 = gv.addEdge(edge_id,gv.getNode(edge->getOrig()->getInfo()),gv.getNode(edge->getDest()->getInfo()),GraphViewer::Edge::EdgeType::DIRECTED);
            edge1.setColor(GraphViewer::RED);
            edge_id++;
        }
    }

        /*Node<int>*node = dynamic_cast<Node<int> *>(graph.findVertex(122525234));



         N &vertex_node = gv.addNode(node->getInfo(),
                                     sf::Vector2f((node->getLongitude() - min_lon) / (max_lon - min_lon) * 900,
                                                  (node->getLatitude() - min_lat) / (max_lat - min_lat) *
                                                  900)); //add nodes from path

         for(int i=0;i<graph.findVertex(122525234)->getOutgoing().size();i++){
             Node<int>*node2 = dynamic_cast<Node<int> *>((graph.findVertex(122525234))->getOutgoing()[i]->getDest());
             N &vertex_node2 = gv.addNode(node2->getInfo(),
                                         sf::Vector2f((node2->getLongitude() - min_lon) / (max_lon - min_lon) * 900,
                                                      (node2->getLatitude() - min_lat) / (max_lat - min_lat) *
                                                      900)); //add nodes from path
             E  &edge0 = gv.addEdge(0,vertex_node,vertex_node2,GraphViewer::Edge::EdgeType::DIRECTED);
             edge0.setColor(GraphViewer::BLUE);
         }*/


    /*Node<int>*node2 = dynamic_cast<Node<int> *>(graph.findVertex(138231433));

    N &vertex_node2 = gv.addNode(node2->getInfo(),
                                sf::Vector2f((node2->getLongitude() - min_lon) / (max_lon - min_lon) * 900,
                                             (node2->getLatitude() - min_lat) / (max_lat - min_lat) *
                                             900)); //add nodes from path*/



        /*if (find(parkingNodes.begin(), parkingNodes.end(), vertex_node) != parkingNodes.end()) {
            vertex_node.setColor(GraphViewer::BLUE); //if node is a park, it will be colored blue
        } else {
            vertex_node.setColor(GraphViewer::GREEN); //if node isn't a park, it will be colored green
        }*/
        //vertex_node.setColor(GraphViewer::GREEN); //if node isn't a park, it will be colored green

  //  }

   /* for (Node<int> *node:parkingNodes) {
        N &path_node = gv.addNode(node->getInfo(),
                                  sf::Vector2f((node->getLongitude()-min_lon)/(max_lon-min_lon)*900,(node->getLatitude()-min_lat)/(max_lat-min_lat)*900)); //add nodes from path
        path_node.setColor(GraphViewer::ORANGE);
    }*/

        cout<<counter<<endl;

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

void showConnectedNodes(vector<Node<int> *> connectedNodes){
    GraphViewer gv;
    //Set coordinates of window center
    //gv->setCenter(sf::Vector2f(450, 450));
    float min_lon = -8.6889783;
    float min_lat=41.1385607;
    float max_lon=-8.5545839;
    float max_lat=41.1858236;

    for (Node<int> *node:connectedNodes) {
        cout<<node->getInfo()<<endl;
        N &path_node = gv.addNode(node->getInfo(),
                                  sf::Vector2f((node->getLongitude()-min_lon)/(max_lon-min_lon)*900,(node->getLatitude()-min_lat)/(max_lat-min_lat)*900)); //add nodes from path
        path_node.setColor(GraphViewer::ORANGE);
    }

    gv.setBackground("../Mapa da cidade do Porto-20210505/porto_full.png");

    // Create window
    gv.createWindow(900, 900);
    // Join viewer thread (blocks till window closed)
    gv.join();
}