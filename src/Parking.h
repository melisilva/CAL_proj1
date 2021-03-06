//
// Created by henri on 12/05/2021.
//

#ifndef PROJECT_PARKING_H
#define PROJECT_PARKING_H


class Parking {
protected:
    bool userWillStop = false;
    int userTime = 0;
public:
    virtual bool isFixedPrice() {};

    virtual int getPrice(int time) {};

    bool getUserWillStop() const { return userWillStop; }

    void setUserWillStop(bool willStop) { userWillStop = willStop; }

    int getUserTime() const { return userTime; }

    void setUserTime(int newTime) { userTime = newTime; }

    double getUserCost() { return getPrice(userTime); }

//    double getUserCost(int time) { return getPrice(time); }
};


#endif //PROJECT_PARKING_H
