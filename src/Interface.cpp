//
// Created by 35191 on 10/05/2021.
//
#include "Interface.h"
#include <iostream>
#include "Graph.h"

using namespace std;

void Interface::begin() {
    cout << "WELCOME TO PARKING FINDER v1.0\n";
    cout << "This app is here to help you find the best path to take from a start point P to a destination D.\n";
    cout << "For that, we need you to provide your desired P and D.\n";
    cout << "To help you, know that point (x1, y1) is at the upper left corner and (x2, y2) at the lower right corner.\n";
    cout << "Use this information to provide an estimated coordinates of your desired points - we'll take care of the rest.\n";
}

pair<double, double> getP() {
    double x = 0.0, y = 0.0;
    while (!cin.fail()) {
        cout << "Please provide an X coordinate for P: ";
        cin >> x;
    }

    while (!cin.fail()) {
        cout << "Please provide an X coordinate for P: ";
        cin >> y;
    }

    return make_pair(x, y);
}

pair<double, double> getD() {
    double x = 0.0, y = 0.0;
    while (!cin.fail()) {
        cout << "Please provide an X coordinate for D: ";
        cin >> x;
    }

    while (!cin.fail()) {
        cout << "Please provide an X coordinate for D: ";
        cin >> y;
    }

    return make_pair(x, y);
}

double calculateDistance(double x1, double y1, double x2, double y2) {
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

template<class T>
pair<T, T> findNodes(Graph<T> graph, pair<double, double> P, pair<double, double> D) {
    T p = NULL, d = NULL;

    for (int i = 0; i < graph.getVertexSet().size(); i++) {
        if (i == 0) {
            p = graph.getVertexSet()[0];
            d = graph.getVertexSet()[0];
            continue;
        }

        double x1 = P.first, y1 = P.second, x2 = D.first, y2 = D.second;
        double x3 = graph.getVertexSet()[i]->getInfo().first;
        double y3 = graph.getVertexSet()[i]->getInfo().second;
        double x4 = p.getInfo().getLatitude(), y4 = p.getInfo().getLongitude();
        double x5 = d.getInfo().getLatitude(), y5 = d.getInfo().getLongitude();

        if (abs(calculateDistance(x1, y1, x3, y3)) < abs(calculateDistance(x1, y1, x4, y4))) {
            p = graph.getVertexSet()[i];
        } else if (abs(calculateDistance(x2, y2, x3, y3)) < abs(calculateDistance(x1, y1, x5, y5))) {
            d = graph.getVertexSet()[i];
        }
    }

    return make_pair(p, d);
}

void Interface::app() {
    begin();
    execute();
}

void Interface::execute() {
    int option;
    while (1) {
        displayOptions();
        cin >> option;
        switch (option) {

            case 0:
                return;
            case 1:
                chooseWeights();
                break;
            case 2:
                start = chooseStart();
                break;
            case 3:
                intermediary = chooseIntermediary();
                break;
            case 4:
                end = chooseEnd();
                break;
            case 5:
                startAlgo();
            default:
                cout << "Please choose a viable option\n";
        }

    }
}

void Interface::displayOptions() const {
    cout << "0: Exit\n";
    cout << "1: Choose Weights\n";
    cout << "2: Choose Start\n";
    cout << "3: Choose Intermediary\n";
    cout << "4: Choose End\n";
    cout << "5: Calculate Path\n";
}

void Interface::chooseWeights() {
    float a, b, c;
    cout << "Choose weights\n";
    cout << "Driving distance\n";
    cin >> a;
    cout << "Parking price\n";
    cin >> b;
    cout << "Walking distance\n";
    cin >> c;
    float sum = a + b + c;
    algo.setWeights(a/sum, b/sum, c/sum);
}

Node<int> * Interface::chooseStart() const {

}

vector<pair<bool, Node<int>*>> Interface::chooseIntermediary() const {

}

Node<int> * Interface::chooseEnd() const {

}

void Interface::startAlgo() {
    intermediary.push_back(pair<bool, Node<int>*>(parkAtEnd, end));
    algo.execute(start, intermediary);
    intermediary.pop_back();
}

