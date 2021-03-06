//
// Created by henri on 12/05/2021.
//

#ifndef PROJECT_PATH_H
#define PROJECT_PATH_H

#include <vector>
#include "GeneralPath.h"

using namespace std;

class Path : public GeneralPath {
    vector<Node<int> *> path;
public:

    Path(Node<int> *n);

    void displayPath() const;

    vector<Node<int> *> getAllNodes();

    void appendPath(Node<int> *path, double weight);

    void appendPath(Node<int> *path);

    void setLength(double length);

    double getLength();

    GeneralPath * reverse();
};


#endif //PROJECT_PATH_H
