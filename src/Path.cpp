//
// Created by henri on 12/05/2021.
//

#include "Path.h"

void Path::displayPath() const {
    for(auto node: path){
        node->displayNode();
    }
}

void Path::appendPath(Node<int> *node, double weight) {
    last = node;
    length += weight;
}

void Path::setCarOnly(bool isCarOnly) {
    carOnly = isCarOnly;
}

void Path::setWalkOnly(bool isWalkOnly) {
    walkOnly = isWalkOnly;
}
