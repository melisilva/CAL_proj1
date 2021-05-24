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

void showConnectedNodes(vector<Node<int> *> connectedNodes,Graph<int> graph){
    GraphViewer gv;
    vector<int> graph_nodes;
    float min_lon = -8.6889783;
    float min_lat=41.1385607;
    float max_lon=-8.5545839;
    float max_lat=41.1858236;
    int edge_id =0;

    for (Node<int> *node:connectedNodes) {
        if (find(graph_nodes.begin(), graph_nodes.end(), node->getInfo()) == graph_nodes.end()) {
            N &path_node = gv.addNode(node->getInfo(),
                                      sf::Vector2f((node->getLongitude() - min_lon) / (max_lon - min_lon) * 900,
                                                   (node->getLatitude() - min_lat) / (max_lat - min_lat) *
                                                   900)); //add nodes from path

            path_node.setColor(GraphViewer::GREEN);
            path_node.setSize(5.0);


            graph_nodes.push_back(node->getInfo());
        }
    }

    for(Node<int>*node:connectedNodes){
        Vertex<int>*v = graph.findVertex(node->getInfo());

        for(Edge<int> *edge:v->getOutgoing()) {
            if (find(graph_nodes.begin(), graph_nodes.end(), edge->getOrig()->getInfo()) != graph_nodes.end() &&
                find(graph_nodes.begin(), graph_nodes.end(), edge->getDest()->getInfo()) != graph_nodes.end()) {
                E &edge0 = gv.addEdge(edge_id, gv.getNode(edge->getOrig()->getInfo()),
                                      gv.getNode(edge->getDest()->getInfo()), GraphViewer::Edge::EdgeType::UNDIRECTED);
                edge0.setColor(GraphViewer::BLACK);
                edge_id++;
            }
        }

        for(Edge<int> *edge:v->getIncoming()) {
            if (find(graph_nodes.begin(), graph_nodes.end(), edge->getOrig()->getInfo()) != graph_nodes.end() &&
                find(graph_nodes.begin(), graph_nodes.end(), edge->getDest()->getInfo()) != graph_nodes.end()) {
                E &edge1 = gv.addEdge(edge_id, gv.getNode(edge->getOrig()->getInfo()),
                                      gv.getNode(edge->getDest()->getInfo()), GraphViewer::Edge::EdgeType::UNDIRECTED);
                edge1.setColor(GraphViewer::BLACK);
                edge_id++;
            }
        }
    }

   // gv.setBackground("../Mapa da cidade do Porto-20210505/porto_strong_component.png");

    // Create window
    gv.createWindow(900, 900);
    // Join viewer thread (blocks till window closed)
    gv.join();
}

void showPath(vector<Node<int> *> path,Graph<int> graph){
    GraphViewer gv;
    vector<int> graph_nodes;
    int edge_id= 0;
    //Set coordinates of window center
    //gv->setCenter(sf::Vector2f(450, 450));
    float min_lon = -8.6889783;
    float min_lat=41.1385607;
    float max_lon=-8.5545839;
    float max_lat=41.1858236;

    for (Node<int> *node:path) {
        if (find(graph_nodes.begin(), graph_nodes.end(), node->getInfo()) == graph_nodes.end()) {
            N &path_node = gv.addNode(node->getInfo(),
                                      sf::Vector2f((node->getLongitude() - min_lon) / (max_lon - min_lon) * 900,
                                                   (node->getLatitude() - min_lat) / (max_lat - min_lat) *
                                                   900)); //add nodes from path
            if(node->getParking()){
                path_node.setColor(GraphViewer::BLUE);
                path_node.setSize(10.0);
            }
            else{
                path_node.setColor(GraphViewer::GREEN);
                path_node.setSize(5.0);
            }

            graph_nodes.push_back(node->getInfo());
        }
    }



    for(Node<int>*node:path){
        Vertex<int>*v = graph.findVertex(node->getInfo());

        for(Edge<int> *edge:v->getOutgoing()) {
            if (find(graph_nodes.begin(), graph_nodes.end(), edge->getOrig()->getInfo()) != graph_nodes.end() &&
                find(graph_nodes.begin(), graph_nodes.end(), edge->getDest()->getInfo()) != graph_nodes.end()) {
                E &edge0 = gv.addEdge(edge_id, gv.getNode(edge->getOrig()->getInfo()),
                                      gv.getNode(edge->getDest()->getInfo()), GraphViewer::Edge::EdgeType::UNDIRECTED);
                edge0.setColor(GraphViewer::BLACK);
                edge_id++;
            }
        }

        for(Edge<int> *edge:v->getIncoming()) {
            if (find(graph_nodes.begin(), graph_nodes.end(), edge->getOrig()->getInfo()) != graph_nodes.end() &&
                find(graph_nodes.begin(), graph_nodes.end(), edge->getDest()->getInfo()) != graph_nodes.end()) {
                E &edge1 = gv.addEdge(edge_id, gv.getNode(edge->getOrig()->getInfo()),
                                      gv.getNode(edge->getDest()->getInfo()), GraphViewer::Edge::EdgeType::UNDIRECTED);
                edge1.setColor(GraphViewer::BLACK);
                edge_id++;
            }
        }
    }

    //gv.setBackground("../Mapa da cidade do Porto-20210505/porto_strong_component.png");

    // Create window
    gv.createWindow(900, 900);
    // Join viewer thread (blocks till window closed)
    gv.join();
}

void twoPoints(Node<int>* v1, Node<int>*v2) {
    GraphViewer gv;
    vector<int> graph_nodes;
    float min_lon = -8.6889783;
    float min_lat=41.1385607;
    float max_lon=-8.5545839;
    float max_lat=41.1858236;



    Node<int> *node1 = dynamic_cast<Node<int> *>(v1);
            N &vertex_node1 = gv.addNode(node1->getInfo(),
                                        sf::Vector2f((node1->getLongitude() - min_lon) / (max_lon - min_lon) * 900,
                                                     (node1->getLatitude() - min_lat) / (max_lat - min_lat) *
                                                     900)); //add nodes from path
    vertex_node1.setColor(GraphViewer::GREEN);
    vertex_node1.setSize(10.0);

    Node<int> *node2 = dynamic_cast<Node<int> *>(v2);
    N &vertex_node2 = gv.addNode(node2->getInfo(),
                                 sf::Vector2f((node2->getLongitude() - min_lon) / (max_lon - min_lon) * 900,
                                              (node2->getLatitude() - min_lat) / (max_lat - min_lat) *
                                              900)); //add nodes from path
    vertex_node2.setColor(GraphViewer::RED);
    vertex_node2.setSize(10.0);





   // gv.setBackground("../Mapa da cidade do Porto-20210505/porto_strong_component.png");


    // Create window
    gv.createWindow(900, 900);
    // Join viewer thread (blocks till window closed)
    gv.join();
}

void showParkingLots(Graph<int> graph){
    GraphViewer gv;
    vector<int> graph_nodes;
    float min_lon = -8.6889783;
    float min_lat=41.1385607;
    float max_lon=-8.5545839;
    float max_lat=41.1858236;
    int edge_id =0;

    for (Vertex<int> *vertex: graph.getVertexSet()) { //graph's nodes/vertexes

        Node<int> *node = dynamic_cast<Node<int> *>(vertex);
        if(node->getParking()) {
            N &vertex_node = gv.addNode(node->getInfo(),
                                        sf::Vector2f((node->getLongitude() - min_lon) / (max_lon - min_lon) * 900,
                                                     (node->getLatitude() - min_lat) / (max_lat - min_lat) *
                                                     900)); //add nodes from path
        vertex_node.setColor(GraphViewer::CYAN);
        vertex_node.setSize(40.0);
        vertex_node.setLabel(to_string(vertex_node.getId()));
        }


    }

        //gv.setBackground("../Mapa da cidade do Porto-20210505/porto_strong_component.png");


    // Create window
    gv.createWindow(900, 900);
    // Join viewer thread (blocks till window closed)
    gv.join();
}



