//
// Created by 35191 on 10/05/2021.
//

#ifndef PROJECT_Interface<T>_H
#define PROJECT_Interface<T>_H

#include "Algorithm.h"

template <class T>
class Interface{
    Algorithm algo;
public:
    Interface<T>(Algorithm algo): algo(algo){}
    void begin();
    void app();
    void execute();
    void displayOptions() const;
    void chooseWeights() const;
    Node<T>* chooseNode() const;
    vector<pair<int, Node<T>*>> chooseIntermediary() const;
    int getNodeOption();
    void showNodeOptions() const;
    double calcNodeDistance(Node<T> a, Node<T> b);
    Node<T>* getNodeID() const;
    Node<T>* getNodeCoordinates();

    void getRandomNode() const;
};


#endif //PROJECT_Interface<T>_H
