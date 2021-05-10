//
// Created by 35191 on 10/05/2021.
//
#include "Interface.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <iostream>
using namespace std;

void begin(){
    cout << "WELCOME TO PARKING FINDER v1.0\n";
    cout << "This app is here to help you find the best path to take from a start point P to a destination D.\n";
    cout << "For that, we need you to provide your desired P and D.\n";
    cout << "To help you, know that point (x1, y1) is at the upper left corner and (x2, y2) at the lower right corner.\n";
    cout << "Use this information to provide an estimated coordinates of your desired points - we'll take care of the rest.\n";
}

pair<double, double> getP(){
    double x = 0.0, y = 0.0;
    while (!cin.fail()){
        cout << "Please provide an X coordinate for P: ";
        cin >> x;
    }

    while (!cin.fail()){
        cout << "Please provide an X coordinate for P: ";
        cin >> y;
    }

    return make_pair(x, y);
}

pair<double, double> getD(){
    double x = 0.0, y = 0.0;
    while (!cin.fail()){
        cout << "Please provide an X coordinate for D: ";
        cin >> x;
    }

    while (!cin.fail()){
        cout << "Please provide an X coordinate for D: ";
        cin >> y;
    }

    return make_pair(x, y);
}

void findNodes(pair<double, double> P, pair<double, double> D){

}

void app(){
    begin();
    findNodes(getP(), getD());
}

