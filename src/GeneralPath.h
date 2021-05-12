//
// Created by henri on 12/05/2021.
//

#ifndef PROJECT_GENERALPATH_H
#define PROJECT_GENERALPATH_H

#include "Graph.h"

class GeneralPath {
protected:
    double length;
    bool carOnly;
    bool walkOnly;
    Node<int> *first;
    Node<int> *last;
public:
    virtual void displayPath() const = 0;

    double getLength() const;

    Node<int> *getFirst() const;

    Node<int> *getLast() const;

    bool getCarOnly() const;

    bool getWalkOnly() const;
};


#endif //PROJECT_GENERALPATH_H
