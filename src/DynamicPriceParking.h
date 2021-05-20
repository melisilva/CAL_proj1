//
// Created by henri on 12/05/2021.
//

#ifndef PROJECT_DYNAMICPRICEPARKING_H
#define PROJECT_DYNAMICPRICEPARKING_H

#include "stdlib.h"
#include "cmath"
#include "Parking.h"

class DynamicPriceParking : public Parking {
protected:
    int centsPerHour;
    int occupied;
    int capacity;
    float maxMult;

public:
    DynamicPriceParking() : Parking() {
        centsPerHour = rand() % 200 + 1;
        capacity = rand() % 1000 + 1;
        occupied = rand() % capacity + 1;
        maxMult = rand() % 300 / 100;

    }

    bool isFixedPrice() {
        return true;
    };

    int getPrice(int time) {

        int vacant = capacity - occupied;
        int timeFactor = centsPerHour * ((time + 59) / 60);
        int occupancyFactor = maxMult * (vacant) * (vacant - 2);
        return timeFactor * occupancyFactor;
    };
};


#endif //PROJECT_DYNAMICPRICEPARKING_H
