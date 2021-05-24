//
// Created by Melissa Silva on 14/05/2021.
//

#ifndef PROJECT_VIEWGRAPH_H
#define PROJECT_VIEWGRAPH_H

#include "Node.h"
#include "Graph.h"

void showConnectedNodes(vector<Node<int> *> connectedNodes, Graph<int> graph);

void showParkingLots(Graph<int> graph);

void twoPoints(Node<int> *v1, Node<int> *v2);

void showPath(vector<Node<int> *> path, Graph<int> graph);

#endif //PROJECT_VIEWGRAPH_H
