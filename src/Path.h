//
// Created by henri on 12/05/2021.
//

#ifndef PROJECT_PATH_H
#define PROJECT_PATH_H

#include <vector>
#include "GeneralPath.h"
#include "Graph.h"

using namespace std;
class Path: public GeneralPath {
    vector<Node<int> *> path;

    void displayPath() const;
    void appendToPath(Node<int>* node);
};


#endif //PROJECT_PATH_H
