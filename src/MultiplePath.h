//
// Created by henri on 12/05/2021.
//

#ifndef PROJECT_MULTIPLEPATH_H
#define PROJECT_MULTIPLEPATH_H

#include <vector>
#include "GeneralPath.h"
#include "Path.h"

using namespace std;

class MultiplePath: public GeneralPath {
    vector<Path> fullPath;
public:
    void displayPath() const{};
    virtual void appendPath(GeneralPath* path) const { };
    virtual Node<int>* getFirst() const { };
    virtual Node<int>* getLast() const { };
    virtual Node<int>* getCarOnly() const { };
    virtual Node<int>* getWalkOnly() const { };

};


#endif //PROJECT_MULTIPLEPATH_H
