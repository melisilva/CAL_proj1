//
// Created by henri on 12/05/2021.
//

#ifndef PROJECT_MULTIPLEPATH_H
#define PROJECT_MULTIPLEPATH_H

#include <vector>
#include "GeneralPath.h"
#include "Path.h"

using namespace std;

class MultiplePath : public GeneralPath {
    vector<GeneralPath *> fullPath;
public:
    MultiplePath(Node<int> *n);

    vector<Node<int> *> getAllNodes();

    void displayPath() const;

    void appendPath(GeneralPath *path);

    void removeLastSubPath();

    vector<GeneralPath*> getPath();

    GeneralPath *getLastSubPath();

};


#endif //PROJECT_MULTIPLEPATH_H
