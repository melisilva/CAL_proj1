//
// Created by 35191 on 10/05/2021.
//

#ifndef PROJECT_Interface_H
#define PROJECT_Interface_H

#include "Node.h"
#include "Algorithm.h"

template <class T>
class Interface{
    Algorithm algo;
    Node<int> *start, *end;
    bool parkAtEnd;
    vector<pair<int, Node<int> *>> intermediary;
public:
    Interface(Algorithm algo): algo(algo){}
    void begin();
    void execute();
    void displayOptions() const;
    void chooseWeights();
    Node<T>* chooseNode() const;
    vector<pair<int, Node<T>*>> chooseIntermediary() const;
    int getNodeOption();
    void showNodeOptions() const;
    double calcNodeDistance(Node<T> a, Node<T> b);
    Node<T>* getNodeID() const;
    Node<T>* getNodeCoordinates();
    Node<T>* getRandomNode() const;

    void startAlgo();
};


#endif //PROJECT_Interface<T>_H
