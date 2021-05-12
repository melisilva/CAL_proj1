//
// Created by henri on 12/05/2021.
//

#ifndef PROJECT_PARKING_H
#define PROJECT_PARKING_H

#include "Node.h"
template <class T>
class Parking: public Node<T> {
public:
    Parking(const Node<T>&n):  Node<T>(n){}
    virtual bool isFixedPrice() {};
    virtual int getPrice(int time) {};
};


#endif //PROJECT_PARKING_H
