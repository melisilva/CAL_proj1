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
#include <fstream>
#include <algorithm>
#include <random>
#include "Graph.h"
#include "Node.h"
#include "Algorithm.h"
#include "FileManagement.h"

using namespace std;

template<class T>
class Interface;

void showConnectedNodes(vector<Node<int> *> connectedNodes, Graph<int> graph);

void showParkingLots(Graph<int> graph);

void twoPoints(Node<int> *v1, Node<int> *v2);

template<class T>
class Interface {
    Algorithm algo;
    Node<int> *start, *end, *og_point, *point1, *point2;
    int parkAtEnd;
    long long int elapsed;
    vector<pair<int, Node<int> *>> intermediary;
public:
    Interface(Algorithm algo) : algo(algo) {
        start = nullptr;
        end = nullptr;
        parkAtEnd = -1;
    }

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

    int getRandomParkingTime();

    void startAlgoRandom();

    void startAlgo();

    void startPerformanceAnalysis();
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
                startAlgo();
                option = 0;
                break;
            case 3:
                showConnectivity();
                break;
            case 4:
                showConnectivityFromPoint();
                break;
            case 5:
                start = getRandomNode();
                end = getRandomNode();
                startAlgoRandom();
                break;
            case 6:
                start = getRandomNode();
                for (int i = 0; i < 10; i++) {
                    intermediary.push_back(make_pair(getRandomTime(), getRandomNode()));
                }
                end = getRandomNode();
                startAlgoRandom();
                break;
            case 7:
                startPerformanceAnalysis();
                break;
            case 8:
                showParkingLots(algo.getGraph());
                break;
            case 9:
                point1 = chooseNode();
                point2 = chooseNode();
                twoPoints(point1, point2);
            default:
                cout << "Please choose a viable option.\n";
        }

    }
}

template<class T>
Node<T> *Interface<T>::getRandomNode() {
    int size = algo.getGraph().getVertexSet().size();
    return dynamic_cast<Node<T> *>(algo.getGraph().getVertexSet().at(rand() % size));
}

template<class T>
int Interface<T>::getRandomTime() {
    int doesntParkOneInX = 3;
    int meanParkTime = 10;
    int willItNotPark = rand() % doesntParkOneInX;
    if (willItNotPark) {
        return rand() % 10 + 10;
    } else {
        return 0;
    }
}

template<class T>
int Interface<T>::getRandomParkingTime() {
    return rand() % 20 + 1;
}

template<class T>
void Interface<T>::displayOptions() const {
    cout << "INPUT YOUR DESIRED OPTION\n";
    cout << "0 - Exit\n";
    cout << "1 - Choose Weights\n";
    cout << "2 - Calculate Path\n";
    cout << "3 - Show Graph Connectivity\n";
    cout << "4 - Show Graph Connectivity from chosen point\n";
    cout << "5 - Calculate random Path no intermediary\n";
    cout << "6 - Calculate random Path with intermediary\n";
    cout << "7 - Run performance analysis\n";
    cout << "8 - Show Parking Lots\n";
    cout << "9 - See two points\n";
}

template<class T>
void Interface<T>::showNodeOptions() const {
    cout << "CHOOSE HOW YOU'LL DO YOUR INPUT\n";
    cout << "1 - Node IDs.\n";
    cout << "2 - (lat, long) style coordinates.\n";
    cout << "3 - Random.\n\n";
}

template<class T>
void Interface<T>::showConnectivity() const {
//    for(auto v:algo.getGraph().getVertexSet()){
//        for(auto e:v->getIncoming()){
//            cout<<e->getOrig()->getInfo()<<" "<<e->getDest()->getInfo()<<endl;
//        }
//    }
    cout << "Calculating connectivity\n";
    fflush(stdout);
    auto direct = algo.getGraph();
    vector<int> first =direct.dfs();
    cout << first.size() << endl;
    //Graph<int>*inverted=algo.getGraph().invert();
    Graph<int> inverted;
    readNodesFile(inverted, "../Mapa da cidade do Porto-20210505/porto_full_nodes_latlng.txt");
    readInvertedEdgesFile(inverted, false, "../Mapa da cidade do Porto-20210505/porto_full_edges.txt");

    cout << "INVERTED" << endl;
    fflush(stdout);
//    for(auto v:inverted.getVertexSet()){
//        cout<<v->getOutgoing().size()<<endl;
//       for(auto e:v->getIncoming()){
//           cout<<e->getOrig()->getInfo()<<" "<<e->getDest()->getInfo()<<endl;
//       }
//    }
    inverted.orderForDfs(direct);
    cout << first.at(first.size() - 1) << endl;
    auto regions = inverted.dfsRegions();
    int noTrees = 0;
    vector<vector<Vertex<T> *>> trees;
    for (auto v: inverted.getVertexSet()) {
        if (v->getPath() == nullptr) {
            // This is the root of a tree
            noTrees++;
        }
    }
    cout << "There are " << regions.size() << "strongly connex components\n";
    cout << "There are " << noTrees << " strongly connex components\n";


    /*Vertex<T>  *initial = inverted.findVertex(first.at(first.size()-1));
    vector<int> second = inverted.dfs_inverted(initial);
    cout<<second.size()<<endl;
    vector<int> connected;
    for(auto v:first) {
        vector<int>::iterator it = find (second.begin(), second.end(), v);
        if (it != second.end()){
            connected.push_back(v);
        }
    }




    sort(connected.begin(), connected.end());
    connected.erase(unique(connected.begin(), connected.end()), connected.end());
    vector<Node<int> *> connectedNodes;
    for (int i = 0; i < connected.size(); i++) {

        connectedNodes.push_back(dynamic_cast<Node<int> *>(algo.getGraph().findVertex(connected[i])));
    }
    showConnectedNodes(connectedNodes, algo.getGraph());
*/
   cout<<algo.getGraph().findArt(algo.getGraph().findVertex(1))<<endl;


}

template<class T>
void Interface<T>::showConnectivityFromPoint() {
    og_point = chooseNode();
    vector<int> connected = algo.getGraph().bfs(og_point);
    vector<Node<int> *> connectedNodes;
    for (int i = 0; i < connected.size(); i++) {
        connectedNodes.push_back(dynamic_cast<Node<int> *>(algo.getGraph().findVertex(connected[i])));
    }
    showConnectedNodes(connectedNodes, algo.getGraph());
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
    cout << "CHOOSE WEIGHTS FOR EACH OF THE FOLLOWING:\n";
    cout << "Driving Distance: ";
    cin >> a;
    cout << endl;
    cout << "Parking Price: ";
    cin >> b;
    cout << endl;
    cout << "Walking Distance: ";
    cin >> c;
    cout << endl;
    float sum = a + b + c;
    algo.setWeights(a / sum, b / sum, c / sum);
}

template<class T>
Node<T> *Interface<T>::getNodeCoordinates() {
    double x = -1.0, y = -1.0;
    while (cin.fail() || x == -1.0) {
        cout << "Please provide a Latitude coordinate for P: ";
        cin >> x;
    }

    while (cin.fail() || y == -1.0) {
        cout << "Please provide a Longitude coordinate for P: ";
        cin >> y;
    }

    Node<T> *parkingNode = new Node<T>(-1, x, y);
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
    while (cin.fail() || temp != "DONE") {
        cout << "If you wish to add an intermediary, input NEW. Otherwise, input DONE.\n";
        cin >> temp;
        Node<T> *newStop;

        if (temp != "DONE") {
            newStop = chooseNode();
            while (answer < 0) {
                cout << "Do you wish to PASS BY (0) in this stop or PARK AND STOP BY (INPUT THE TIME YOU WISH TO SPEND IN PARK): ";
                cin >> answer;
            }

            midStops.push_back(make_pair(answer, newStop));
        }


    }
    return midStops;
}

template<class T>
void Interface<T>::startAlgo() {

    while (start == nullptr) {
        cout << "CHOOSE YOUR STARTING POINT.\n";
        start = chooseNode();
    }

    cout << "CHOOSE YOUR INTERMEDIARY POINT(S).\n";
    intermediary = chooseIntermediary();

    while (end == nullptr) {
        cout << "CHOOSE YOUR ENDING POINT.\n";
        end = chooseNode();
    }

    while (parkAtEnd < 0) {
        cout << "EXPECTED PARKING TIME: ";
        cin >> parkAtEnd;
    }

    intermediary.push_back(pair<int, Node<int> *>(parkAtEnd, end));
    algo.visualizeExecute(start, intermediary, elapsed);
    intermediary.pop_back();
}

template<class T>
void Interface<T>::startAlgoRandom() {
    parkAtEnd = getRandomTime();
    intermediary.push_back(pair<int, Node<int> *>(parkAtEnd, end));
    algo.visualizeExecute(start, intermediary, elapsed);
    intermediary.pop_back();
}

template<class T>
void Interface<T>::startPerformanceAnalysis() {
    ofstream complexities;
    vector<long long int> averageElapsed;
    int startIntermediary = 0;
    int noIntermediary = 20;
    int noIterations = 20;
    for (int i = startIntermediary; i < noIntermediary; i++) {
        long long int sumElapsed = 0;
        for (int j = 0; j < noIterations; j++) {
            start = getRandomNode();
            for (int k = 0; k < i; k++) {
                intermediary.push_back(make_pair(getRandomParkingTime(), getRandomNode()));
            }
            end = getRandomNode();
            parkAtEnd = getRandomParkingTime();
            intermediary.push_back(pair<int, Node<int> *>(parkAtEnd, end));
            algo.execute(start, intermediary, elapsed);
            sumElapsed += elapsed;
            intermediary.clear();
        }
        sumElapsed /= noIterations;
        averageElapsed.push_back(sumElapsed);
    }
    complexities.open("./complexities.txt");
    if (!complexities.is_open()) {
        return;
    }
    for (int i = startIntermediary; i < noIntermediary; i++) {
        complexities << i << "," << averageElapsed[i - startIntermediary] << "\n";
    }
    complexities.close();
}

#endif //PROJECT_Interface<T>_H