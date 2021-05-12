//
// Created by 35191 on 10/05/2021.
//

#ifndef PROJECT_INTERFACE_H
#define PROJECT_INTERFACE_H

#include "Node.h"
#include "Algorithm.h"

class Interface {
    Algorithm algo;
    Node<int> *start, *end;
    bool parkAtEnd;
    vector<pair<int, Node<int> *>> intermediary;
public:
    Interface(Algorithm algo) : algo(algo) {}

    void begin();

    void app();

    void execute();

    void displayOptions() const;

    void chooseWeights();

    Node<int> *chooseStart() const;

    vector<pair<int, Node<int> *>> chooseIntermediary() const;

    Node<int> *chooseEnd() const;

    void startAlgo();
};


#endif //PROJECT_INTERFACE_H
