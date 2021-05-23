//
// Created by henri on 12/05/2021.
//

#ifndef PROJECT_FIXEDPRICEPARKING_H
#define PROJECT_FIXEDPRICEPARKING_H

#include <stdlib.h>
#include "Parking.h"

class FixedPriceParking : public Parking {
protected:
    int centsPerHour;

public:
    FixedPriceParking() : Parking() {
        centsPerHour = rand() % 200 + 1;
    }

    bool isFixedPrice() {
        return true;
    };

    int getPrice(int time) {
        return centsPerHour * ((time + 59) / 60);
    };
};


#endif //PROJECT_FIXEDPRICEPARKING_H
