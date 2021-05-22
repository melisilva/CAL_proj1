//
// Created by henri on 12/05/2021.
//

#include "Path.h"

Path::Path(Node<int> *n): GeneralPath(n){
    path.push_back(n);
}

vector<Node<int> *> Path::getAllNodes() {
    return path;
}

void Path::displayPath() const {
    for (auto node: path) {
        node->displayNode();
    }
}


void Path::appendPath(Node<int> *node, double weight) {
    path.push_back(node);
    last = node;
    length += weight;
}

void Path::appendPath(Node<int> *node) {
    path.push_back(node);
    last = node;
}

void Path::setLength(double length) {
    this->length = length;
}

double Path::getLength() {
    return this->length;
}

GeneralPath * Path::reverse() {
    Path *newPath = new Path(last);
    for(int i = path.size()-2; i >= 0; i--){
        newPath->appendPath(path[i]);
    }
    newPath->setLength(length);
    newPath->setWalkOnly(walkOnly);
    newPath->setCarOnly(carOnly);
    return newPath;
}
