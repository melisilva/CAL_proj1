//
// Created by 35191 on 10/05/2021.
//

#include <iostream>
#include <algorithm>
#include <random>
#include "Graph.h"
#include "Interface.h"
using namespace std;

template <class T>
void Interface<T>::begin() {
    cout << "WELCOME TO PARKING FINDER v1.0\n";
    cout << "This app is here to help you find the best path to take from a start point P to a destination D.\n";
    cout << "For that, we need you to provide your desired P and D.\n";
    cout << "Here are the desired options to do that.\n";
    cout << endl << endl;
}

template <class T>
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
            default:
                cout << "Please choose a viable option\n";
        }

    }
}

template<class T>
Node<T>* Interface<T>::getRandomNode() const {
    int size = algo.getGraph().getVertexSet().size();
    return dynamic_cast<Node<T>*>(algo.getGraph().getVertexSet().at(rand()%size));
}

template <class T>
void Interface<T>::displayOptions() const {
    cout << "INPUT YOUR DESIRED OPTION\n";
    cout << "0 - Exit\n";
    cout << "1 - Choose Weights\n";
    cout << "2 - Choose Start\n";
    cout << "3 - Choose Intermediary\n";
    cout << "4 - Choose End\n";
    cout << "5 - Calculate Path\n";
}

template <class T>
void Interface<T>::showNodeOptions() const {
    cout << "CHOOSE HOW YOU'LL DO YOUR INPUT\n";
    cout << "1 - Node IDs.\n";
    cout << "2 - (x, y) style coordinates.\n";
    cout << "3 - Random.\n\n";
}

template <class T>
int Interface<T>::getNodeOption(){
    int option = -1;

    while (option != 1 && option != 2 && option != 3){
        cout << "Input your choice: ";
        cin >> option;
    }

    return option;
}

template <class T>
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
    algo.setWeights(a/sum, b/sum, c/sum);
}

template <class T>
double Interface<T>::calcNodeDistance(Node<T> a, Node<T> b){
    return sqrt(pow(a.getLatitude() - b.getLatitude(), 2) + pow(a.getLongitude() - b.getLongitude(), 2));
}

template <class T>
Node<T>* Interface<T>::getNodeCoordinates(){
    double x = 0.0, y = 0.0;
    while (!cin.fail()){
        cout << "Please provide an X coordinate for P: ";
        cin >> x;
    }

    while (!cin.fail()){
        cout << "Please provide an X coordinate for P: ";
        cin >> y;
    }

    Node<T> parkingNode(-1, x, y);
    vector<Vertex<T>*> realNodes = algo.getGraph().getVertexSet();
    sort(realNodes.begin(),
         realNodes.end(),
         [parkingNode, this](Vertex<T> *aRealParkingNodeA, Vertex<T> *aRealParkingNodeB) {
            return calcNodeDistance(dynamic_cast<Node<T>*>(&aRealParkingNodeA), parkingNode) < calcNodeDistance(dynamic_cast<Node<T>*>(&aRealParkingNodeB), parkingNode);
         });

    return realNodes.at(0);
}

template <class T>
Node<T>* Interface<T>::getNodeID() const{
    int x = 0;
    while (!cin.fail()){
        cout << "Please provide an : ";
        cin >> x;
    }

    return dynamic_cast<Node<T> *>(algo.getGraph().findVertex(x));
}

template <class T>
Node<T> * Interface<T>::chooseNode() const {
    showNodeOptions();
    int option = getNodeOption();

    switch (option){
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

template <class T>
vector<pair<int, Node<T> *>> Interface<T>::chooseIntermediary() const {
    string temp = "";
    int answer = 2;
    vector<Node<T>*> midStops;
    do {
        cout << "If you wish to add an intermediary, input NEW. Otherwise, input DONE.\n";
        cin >> temp;
        Node<T>* newStop = chooseNode();

        while (answer != 0 && answer != 1){
            cout << "Do you wish to PASS BY (0) in this stop or PARK AND STOP BY (1):";
            cin >> answer;
        }

        midStops.push_back(make_pair(answer, newStop));
    } while (!cin.fail() && temp != "DONE");
    return midStops;
}

template <class T>
void Interface<T>::startAlgo() {
    intermediary.push_back(pair<bool, Node<int>*>(parkAtEnd, end));
    algo.execute(start, intermediary);
    intermediary.pop_back();
}

