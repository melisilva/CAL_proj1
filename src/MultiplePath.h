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
    void displayPath() const;
    void appendToPath(GeneralPath newPath);
};


#endif //PROJECT_MULTIPLEPATH_H
