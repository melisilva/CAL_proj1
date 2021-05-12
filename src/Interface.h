//
// Created by 35191 on 10/05/2021.
//

#ifndef PROJECT_INTERFACE_H
#define PROJECT_INTERFACE_H

#include "Algorithm.h"

class Interface{
    Algorithm algo;
public:
    Interface(Algorithm algo): algo(algo){}
    void begin();
    void app();
    void execute();
    void displayOptions() const;
    void chooseWeights() const;
    Node<int>*chooseStart() const;
    vector<Node<int>*> chooseIntermediary() const;
    Node<int>*chooseEnd() const;
};


#endif //PROJECT_INTERFACE_H
