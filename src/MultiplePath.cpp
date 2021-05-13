//
// Created by henri on 12/05/2021.
//

#include "MultiplePath.h"

MultiplePath::MultiplePath(Node<int> *n) : GeneralPath(n) {
    length = 0;
    carOnly = true;
    walkOnly = true;
    first = nullptr;
    last = nullptr;
}

vector<Node<int> *> MultiplePath::getAllNodes() {
    vector<Node<int> *> toReturn;
    for (auto path: fullPath) {
        auto toExtend = path->getAllNodes();
        toReturn.reserve(toReturn.size() + distance(toExtend.begin(), toExtend.end()));
        toReturn.insert(toReturn.end(), toExtend.begin(), toExtend.end());
    }
    return toReturn;
}

void MultiplePath::displayPath() const {
    for (auto path: fullPath) {
        path->displayPath();
    }
}

void MultiplePath::appendPath(GeneralPath *path) {
    length += path->getLength();
    carOnly &= path->isCarOnly();
    walkOnly &= path->isWalkOnly();
    last = path->getLast();
    fullPath.push_back(path);
}

void MultiplePath::removeLastSubPath() {
    fullPath.pop_back();
}

vector<GeneralPath *> MultiplePath::getPath() {
    return fullPath;
}

GeneralPath *MultiplePath::getLastSubPath() {
    return fullPath.back();
}