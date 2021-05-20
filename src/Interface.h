//
// Created by 35191 on 10/05/2021.
//


#ifndef PROJECT_Interface_H
#define PROJECT_Interface_H
/*
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
*/


#include <iostream>
#include <algorithm>
#include <random>
#include "Graph.h"
#include "Node.h"
#include "Algorithm.h"

using namespace std;

template<class T>
class Interface;

void showConnectedNodes(vector<Node<int> *> connectedNodes);

template<class T>
class Interface {
    Algorithm algo;
    Node<int> *start, *end, *og_point;
    bool parkAtEnd;
    vector<pair<int, Node<int> *>> intermediary;
public:
    Interface(Algorithm algo) : algo(algo) {}

    void begin();

    void execute();

    void displayOptions() const;

    void chooseWeights();

    Node<T> *chooseNode();

    vector<pair<int, Node<T> *>> chooseIntermediary();

    int getNodeOption();

    void showNodeOptions() const;

    void showConnectivity() const;

    void showConnectivityFromPoint();

    Node<T> *getNodeID();

    Node<T> *getNodeCoordinates();

    Node<T> *getRandomNode();

    int getRandomTime();

    void startAlgo();
};


template<class T>
void Interface<T>::begin() {
    cout << "WELCOME TO PARKING FINDER v1.0\n";
    cout << "This app is here to help you find the best path to take from a start point P to a destination D.\n";
    cout << "For that, we need you to provide your desired P and D.\n";
    cout << "Here are the desired options to do that.\n";
    cout << endl << endl;
}

template<class T>
void Interface<T>::execute() {
    int option;
    while (1) {
        displayOptions();
        cin >> option;
        switch (option) {
            case 0:
                exit(0);
            case 1:
                chooseWeights();
                break;
            case 2:
                start = chooseNode();
                break;
            case 3:
                intermediary = chooseIntermediary();
                break;
            case 4:
                end = chooseNode();
                break;
            case 5:
                startAlgo();
                option = 0;
                break;
            case 6:
                showConnectivity();
                break;
            case 7:
                showConnectivityFromPoint();
                break;
            case 8:
                start = getRandomNode();
                end = getRandomNode();
                startAlgo();
                break;
            case 9:
                start = getRandomNode();
                for(int i = 0; i < 10; i++){
                    intermediary.push_back(make_pair(getRandomTime(), getRandomNode()));
                }
                end = getRandomNode();
                startAlgo();
            default:
                cout << "Please choose a viable option\n";
        }

    }
}

template<class T>
Node<T> *Interface<T>::getRandomNode() {
    int size = algo.getGraph().getVertexSet().size();
    return dynamic_cast<Node<T> *>(algo.getGraph().getVertexSet().at(rand() % size));
}

template<class T>
int     Interface<T>::getRandomTime() {
    int doesntParkOneInX = 3;
    int meanParkTime = 10;
    int willItNotPark = rand() % doesntParkOneInX;
    if(willItNotPark){
        return rand() % 10 + 10;
    }else{
        return 0;
    }
}

template<class T>
void Interface<T>::displayOptions() const {
    cout << "INPUT YOUR DESIRED OPTION\n";
    cout << "0 - Exit\n";
    cout << "1 - Choose Weights\n";
    cout << "2 - Choose Start\n";
    cout << "3 - Choose Intermediary\n";
    cout << "4 - Choose End\n";
    cout << "5 - Calculate Path\n";
    cout << "6 - Show Graph Connectivity\n";
    cout << "7 - Show Graph Connectivity from chosen point\n";
    cout << "8 - Calculate random Path no intermediary\n";
    cout << "9 - Calculate random Path with intermediary\n";
}

template<class T>
void Interface<T>::showNodeOptions() const {
    cout << "CHOOSE HOW YOU'LL DO YOUR INPUT\n";
    cout << "1 - Node IDs.\n";
    cout << "2 - (x, y) style coordinates.\n";
    cout << "3 - Random.\n\n";
}

template<class T>
void Interface<T>::showConnectivity() const {
    vector<int> connected = algo.getGraph().dfs();
    sort(connected.begin(), connected.end());
    connected.erase(unique(connected.begin(), connected.end()), connected.end());
    vector<Node<int> *> connectedNodes;
    for (int i = 0; i < connected.size(); i++) {
        connectedNodes.push_back(dynamic_cast<Node<int> *>(algo.getGraph().findVertex(connected[i])));
    }
    showConnectedNodes(connectedNodes);
}

template<class T>
void Interface<T>::showConnectivityFromPoint() {
    og_point = chooseNode();
    vector<int> connected = algo.getGraph().bfs(og_point);
    vector<Node<int> *> connectedNodes;
    for (int i = 0; i < connected.size(); i++) {
        connectedNodes.push_back(dynamic_cast<Node<int> *>(algo.getGraph().findVertex(connected[i])));
    }
    showConnectedNodes(connectedNodes);
}

template<class T>
int Interface<T>::getNodeOption() {
    int option = -1;

    while (option != 1 && option != 2 && option != 3) {
        cout << "Input your choice: ";
        cin >> option;
    }

    return option;
}

template<class T>
void Interface<T>::chooseWeights() {
    float a, b, c;
    cout << "Choose weights\n";
    cout << "Driving distance\n";
    cin >> a;
    cout << "Parking price\n";
    cin >> b;
    cout << "Walking distance\n";
    cin >> c;
    float sum = a + b + c;
    algo.setWeights(a / sum, b / sum, c / sum);
}

template<class T>
Node<T> *Interface<T>::getNodeCoordinates() {
    double x = -1.0, y = -1.0;
    while (cin.fail() || x == -1.0) {
        cout << "Please provide an X coordinate for P: ";
        cin >> x;
    }

    while (cin.fail() || y == -1.0) {
        cout << "Please provide a Y coordinate for P: ";
        cin >> y;
    }

    Node<T> p(-1, x, y);
    Node<T> *parkingNode;
    *parkingNode = p;
    vector<Vertex<T> *> realNodes = algo.getGraph().getVertexSet();
    sort(realNodes.begin(),
         realNodes.end(),
         [parkingNode, this](Vertex<T> *aRealParkingNodeA, Vertex<T> *aRealParkingNodeB) {
             return parkingNode->calcNodeDistance(dynamic_cast<Node<T> *>(aRealParkingNodeA)) < parkingNode->calcNodeDistance(dynamic_cast<Node<T> *>(aRealParkingNodeB));
         });

    return dynamic_cast<Node<T> *>(realNodes[0]);
}

template<class T>
Node<T> *Interface<T>::getNodeID() {
    int x = -1;
    while (cin.fail() || x == -1) {
        cout << "Please provide an ID: ";
        cin >> x;
    }
    return dynamic_cast<Node<T> *>(algo.getGraph().findVertex(x));


}

template<class T>
Node<T> *Interface<T>::chooseNode() {
    showNodeOptions();
    int option = getNodeOption();

    switch (option) {
        case 1:
            return getNodeID();
            break;
        case 2:
            return getNodeCoordinates();
            break;
        case 3:
            return getRandomNode();
            break;
        default:
            return NULL;
    }
}

template<class T>
vector<pair<int, Node<T> *>> Interface<T>::chooseIntermediary() {
    string temp = "";
    int answer = 2;
    vector<pair<int, Node<T> *>> midStops;
    do {
        cout << "If you wish to add an intermediary, input NEW. Otherwise, input DONE.\n";
        cin >> temp;
        Node<T> *newStop = chooseNode();

        while (answer != 0 && answer != 1) {
            cout << "Do you wish to PASS BY (0) in this stop or PARK AND STOP BY (1):";
            cin >> answer;
        }

        midStops.push_back(make_pair(answer, newStop));
    } while (cin.fail() && temp != "DONE");
    return midStops;
}

template<class T>
void Interface<T>::startAlgo() {
    parkAtEnd = 1;
    cout << "end ";
    end->displayNode();
    intermediary.push_back(pair<int, Node<int> *>(parkAtEnd, end));
    algo.execute(start, intermediary);
    intermediary.pop_back();
}

#endif //PROJECT_Interface<T>_H