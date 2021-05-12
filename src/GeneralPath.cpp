//
// Created by henri on 12/05/2021.
//

#include "GeneralPath.h"

double GeneralPath::getLength() const {
    return length;
}

Node<int> * GeneralPath::getFirst() const {
    return first;
}

Node<int> * GeneralPath::getLast() const {
    return last;
}

bool GeneralPath::getCarOnly() const {
    return carOnly;
}

bool GeneralPath::getWalkOnly() const {
    return walkOnly;
}