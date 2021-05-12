//
// Created by henri on 12/05/2021.
//

#ifndef PROJECT_GENERALPATH_H
#define PROJECT_GENERALPATH_H


class GeneralPath {
    double length;
    bool carOnly;
    bool walkOnly;
    Node<int>* first;
    Node<int>* last;
public:
    virtual void appendPath(GeneralPath* path) const = 0;
    virtual Node<int>* getFirst() const = 0;
    virtual Node<int>* getLast() const = 0;
    virtual Node<int>* getCarOnly() const = 0;
    virtual Node<int>* getWalkOnly() const = 0;
};


#endif //PROJECT_GENERALPATH_H
