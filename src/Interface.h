//
// Created by 35191 on 10/05/2021.
//


#ifndef PROJECT_Interface_H
#define PROJECT_Interface_H


#include <iostream>
#include <fstream>
#include <algorithm>
#include <random>
#include "Graph.h"
#include "Node.h"
#include "Algorithm.h"
#include "FileManagement.h"
#include "ViewGraph.h"

using namespace std;

template<class T>
class Interface;


template<class T>
class Interface {
    Algorithm algo;
    Node<int> *start, *end, *og_point, *point1, *point2;
    int parkAtEnd;
    long long int elapsed, ps_reading, pf_reading, espinho_reading, penafiel_reading;
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

    void comparisonBFS_DFS();
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
                point1=chooseNode();
                point2=chooseNode();
                twoPoints(point1, point2);
            case 10:
                comparisonBFS_DFS();
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
    cout<< "9 - See two points\n";
    cout<< "10 - Comparison BFS and DFS\n";
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
    fflush(stdout);
    Graph<int>direct;
    readNodesFile(direct, "../Mapa da cidade do Porto-20210505/porto_full_nodes_latlng.txt");
    readEdgesFile(direct, false, "../Mapa da cidade do Porto-20210505/porto_full_edges.txt");

    int arti_points = direct.findArt(direct.findVertex(1));

    cout << endl;
    cout << "Number of articulation points " << arti_points << endl;
    cout << "Calculating connectivity\n";


    vector<int> first = direct.dfs();

    Graph<int> inverted;
    readNodesFile(inverted, "../Mapa da cidade do Porto-20210505/porto_full_nodes_latlng.txt");
    readInvertedEdgesFile(inverted, false, "../Mapa da cidade do Porto-20210505/porto_full_edges.txt");


    fflush(stdout);

    inverted.orderForDfs(direct);
    auto regions = inverted.dfsRegions();
    int noTrees = 0;
    vector<vector<Vertex<T> *>> trees;
    for (auto v: inverted.getVertexSet()) {
        if (v->getPath() == nullptr) {
            // This is the root of a tree
            noTrees++;
        }
    }
    cout << "There are " << regions.size() << " strongly connex components\n";
    cout << "There are " << noTrees << " strongly connex components\n";
    sort(regions.begin(), regions.end(), [](const vector<Vertex<T>*> v1, const vector<Vertex<T>*>v2){ return v1.size() > v2.size();});
    for(auto region: regions){
        if(region.size() > 100){
            vector<Node<T>*> nodesRegion;
            for(auto v: region){
                nodesRegion.push_back(dynamic_cast<Node<int> *>(v));
            }
            showConnectedNodes(nodesRegion, direct);
        }
    }
    cout << "Other regions are strongly connected but with a reduced amount of vertices\n";

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
    Vertex<T>* v;
    while (cin.fail() || x == -1) {
        cout << "Please provide an ID: ";
        cin >> x;
        v = algo.getGraph().findVertex(x);
        if(v == nullptr){
            x = -1;
            cout << "Invalid ID" <<endl;
        }
    }

    return dynamic_cast<Node<T> *>(v);
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
    int answer = -2;
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
	    answer = -2;
        }


    }
    return midStops;
}

template<class T>
void Interface<T>::startAlgo() {


        cout << "CHOOSE YOUR STARTING POINT.\n";
        start = chooseNode();


    cout << "CHOOSE YOUR INTERMEDIARY POINT(S).\n";
    intermediary = chooseIntermediary();

        cout << "CHOOSE YOUR ENDING POINT.\n";
        end = chooseNode();


    while (parkAtEnd < 0) {
        cout << "EXPECTED PARKING TIME: ";
        cin >> parkAtEnd;
    }

    intermediary.push_back(pair<int, Node<int> *>(parkAtEnd, end));
    parkAtEnd = -1;
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
        cout << "Intermediary " << i << endl;
        long long int sumElapsed = 0;
        for (int j = 0; j < noIterations; j++) {
            cout << "Iteration " << j << endl;
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

template<class T>
void Interface<T>::comparisonBFS_DFS(){
    Graph<int> espinho_strong, penafiel_strong, porto_full;
    auto startTime = std::chrono::high_resolution_clock::now();
    algo.getGraph().dfs();
    auto finishTime = std::chrono::high_resolution_clock::now();
    ps_reading = std::chrono::duration_cast<std::chrono::microseconds>(finishTime - startTime).count();

    readNodesFile(espinho_strong, "../Mapa da cidade do Porto-20210505/espinho_strong_nodes_latlng.txt");
    readEdgesFile(espinho_strong, false, "../Mapa da cidade do Porto-20210505/espinho_strong_edges.txt");
    startTime =std::chrono::high_resolution_clock::now();
    espinho_strong.dfs();
    finishTime = std::chrono::high_resolution_clock::now();
    espinho_reading = std::chrono::duration_cast<std::chrono::microseconds>(finishTime - startTime).count();

    readNodesFile(penafiel_strong, "../Mapa da cidade do Porto-20210505/penafiel_strong_nodes_latlng.txt");
    readEdgesFile(penafiel_strong, false, "../Mapa da cidade do Porto-20210505/penafiel_strong_edges.txt");
    startTime =std::chrono::high_resolution_clock::now();
    penafiel_strong.dfs();
    finishTime = std::chrono::high_resolution_clock::now();
    penafiel_reading = std::chrono::duration_cast<std::chrono::microseconds>(finishTime - startTime).count();

    readNodesFile(porto_full, "../Mapa da cidade do Porto-20210505/porto_full_nodes_latlng.txt");
    readEdgesFile(porto_full, false, "../Mapa da cidade do Porto-20210505/porto_full_edges.txt");
    startTime =std::chrono::high_resolution_clock::now();
    porto_full.dfs();
    finishTime = std::chrono::high_resolution_clock::now();
    pf_reading = std::chrono::duration_cast<std::chrono::microseconds>(finishTime - startTime).count();

    cout << "Time used for transversing graphs DFS " << ps_reading <<" " << pf_reading << " " << espinho_reading << " "<< penafiel_reading<<" microseconds\n";

    startTime = std::chrono::high_resolution_clock::now();
    algo.getGraph().bfs(dynamic_cast<Node<T> *>(algo.getGraph().getVertexSet().at(0)));
    finishTime = std::chrono::high_resolution_clock::now();
    ps_reading = std::chrono::duration_cast<std::chrono::microseconds>(finishTime - startTime).count();

    startTime =std::chrono::high_resolution_clock::now();
    espinho_strong.bfs(dynamic_cast<Node<T> *>(espinho_strong.getVertexSet().at(0)));
    finishTime = std::chrono::high_resolution_clock::now();
    espinho_reading = std::chrono::duration_cast<std::chrono::microseconds>(finishTime - startTime).count();

    startTime =std::chrono::high_resolution_clock::now();
    penafiel_strong.bfs(dynamic_cast<Node<T> *>(penafiel_strong.getVertexSet().at(0)));
    finishTime = std::chrono::high_resolution_clock::now();
    penafiel_reading = std::chrono::duration_cast<std::chrono::microseconds>(finishTime - startTime).count();

    startTime =std::chrono::high_resolution_clock::now();
    porto_full.bfs(dynamic_cast<Node<T> *>(porto_full.getVertexSet().at(0)));
    finishTime = std::chrono::high_resolution_clock::now();
    pf_reading = std::chrono::duration_cast<std::chrono::microseconds>(finishTime - startTime).count();

    cout << "Time used for transversing graphs BFS " << ps_reading << " " << pf_reading << " " << espinho_reading << " "<< penafiel_reading<<" microseconds\n";


}

#endif //PROJECT_Interface<T>_H