//
// Created by henri on 12/05/2021.
//

#ifndef PROJECT_GENERALPATH_H
#define PROJECT_GENERALPATH_H

#include "Node.h"

class GeneralPath {
protected:
    double length = 0;
    bool carOnly = false;
    bool walkOnly = false;
    Node<int> *first = nullptr;
    Node<int> *last = nullptr;
public:

    GeneralPath(Node<int> *n);

    bool isEmpty() const;

    virtual void displayPath() const = 0;

    virtual vector<Node<int>*> getAllNodes() = 0;

    double getLength() const;

    Node<int> *getFirst() const;

    Node<int> *getLast() const;

    bool isCarOnly() const;

    bool isWalkOnly() const;

    virtual GeneralPath* reverse() = 0;
};


#endif //PROJECT_GENERALPATH_H
