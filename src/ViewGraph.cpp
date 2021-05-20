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



        /*if((dynamic_cast <Parking<int>*> (node)) != NULL){
            counter++;
            vertex_node.setColor(GraphViewer::BLUE); //if node is a park, it will be colored blue
        }
        else{
            vertex_node.setColor(GraphViewer::GREEN); //if node isn't a park, it will be colored green
        }*/
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
            if(node->getParking()){
                path_node.setColor(GraphViewer::BLUE);
            }
            else{
                path_node.setColor(GraphViewer::GREEN);
            }

            graph_nodes.push_back(node->getInfo());
        }
    }

    for(Node<int>*node:connectedNodes){
        Vertex<int>*v = graph.findVertex(node->getInfo());

        for(Edge<int> *edge:v->getOutgoing()) {
            if (find(graph_nodes.begin(), graph_nodes.end(), edge->getOrig()->getInfo()) != graph_nodes.end() &&
                find(graph_nodes.begin(), graph_nodes.end(), edge->getDest()->getInfo()) != graph_nodes.end()) {
                E &edge0 = gv.addEdge(edge_id, gv.getNode(edge->getOrig()->getInfo()),
                                      gv.getNode(edge->getDest()->getInfo()), GraphViewer::Edge::EdgeType::DIRECTED);
                edge0.setColor(GraphViewer::RED);
                edge_id++;
            }
        }

        for(Edge<int> *edge:v->getIncoming()) {
            if (find(graph_nodes.begin(), graph_nodes.end(), edge->getOrig()->getInfo()) != graph_nodes.end() &&
                find(graph_nodes.begin(), graph_nodes.end(), edge->getDest()->getInfo()) != graph_nodes.end()) {
                E &edge1 = gv.addEdge(edge_id, gv.getNode(edge->getOrig()->getInfo()),
                                      gv.getNode(edge->getDest()->getInfo()), GraphViewer::Edge::EdgeType::DIRECTED);
                edge1.setColor(GraphViewer::RED);
                edge_id++;
            }
        }
    }

    gv.setBackground("../Mapa da cidade do Porto-20210505/porto_strong_component.png");

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
            }
            else{
                path_node.setColor(GraphViewer::GREEN);
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
                                      gv.getNode(edge->getDest()->getInfo()), GraphViewer::Edge::EdgeType::DIRECTED);
                edge0.setColor(GraphViewer::RED);
                edge_id++;
            }
        }

        for(Edge<int> *edge:v->getIncoming()) {
            if (find(graph_nodes.begin(), graph_nodes.end(), edge->getOrig()->getInfo()) != graph_nodes.end() &&
                find(graph_nodes.begin(), graph_nodes.end(), edge->getDest()->getInfo()) != graph_nodes.end()) {
                E &edge1 = gv.addEdge(edge_id, gv.getNode(edge->getOrig()->getInfo()),
                                      gv.getNode(edge->getDest()->getInfo()), GraphViewer::Edge::EdgeType::DIRECTED);
                edge1.setColor(GraphViewer::RED);
                edge_id++;
            }
        }
    }

    gv.setBackground("../Mapa da cidade do Porto-20210505/porto_strong_component.png");

    // Create window
    gv.createWindow(900, 900);
    // Join viewer thread (blocks till window closed)
    gv.join();
}


