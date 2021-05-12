//
// Created by henri on 12/05/2021.
//

#include "MultiplePath.h"

MultiplePath::MultiplePath() {
    length = 0;
    carOnly = true;
    walkOnly = true;
    first = nullptr;
    last = nullptr;
}
void MultiplePath::displayPath() const {
    for(auto path: fullPath){
        path->displayPath();
    }
}

void MultiplePath::appendPath(GeneralPath *path) {
    length += path->getLength();
    carOnly &= path->getCarOnly();
    walkOnly &= path->getWalkOnly();
    last = path->getLast();
    fullPath.push_back(path);
}