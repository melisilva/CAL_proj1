//
// Created by henri on 12/05/2021.
//

#include "GeneralPath.h"


GeneralPath::GeneralPath(Node<int> *n) {
    first = n;
}

bool GeneralPath::isEmpty() const {
    return last == nullptr;
}

double GeneralPath::getLength() const {
    return length;
}

Node<int> *GeneralPath::getFirst() const {
    return first;
}

Node<int> *GeneralPath::getLast() const {
    return last;
}

bool GeneralPath::isCarOnly() const {
    return carOnly;
}

bool GeneralPath::isWalkOnly() const {
    return walkOnly;
}

void GeneralPath::setCarOnly(bool isCarOnly) {
    carOnly = isCarOnly;
}

void GeneralPath::setWalkOnly(bool isWalkOnly) {
    walkOnly = isWalkOnly;
}