#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <vector>
#include <set>
#include <algorithm>
#include <cmath>
#include <math.h>

#define _USE_MATH_DEFINES


using namespace std;

class Node {
public:
    double lat, longi;
    int id = -1;

    Node(double lat, double longi) : lat(lat), longi(longi) {}

    Node(int id, double lat, double longi) : id(id), lat(lat), longi(longi) {}
};

//https://stackoverflow.com/a/27943
double deg2Rad(double deg) {
    return deg * (M_PI / 180);
}
//https://stackoverflow.com/a/27943
double calcNodeDistance(const Node &n1, const Node &n2) {
//    return sqrt(pow(n1.lat - n2.lat, 2) + pow(n1.longi - n2.longi, 2));
    double radius = 6371;
    double dLat = deg2Rad(n2.lat - n1.lat);
    double dLon = deg2Rad(n2.longi - n1.longi);
    double a = sin(dLat/2) * sin(dLat/2) +
            cos(deg2Rad(n1.lat)) * cos(deg2Rad(n2.lat)) *
            sin(dLon/2) * sin(dLon/2);
    double c = 2 * atan2(sqrt(a), sqrt(1-a));
    double d = radius * c;
    return d;
}

int main() {

    ifstream parkings;
    ifstream nodes_lat_long;
    ofstream parkingTags;
    string line;
    stringstream iLine;
    double lat, longi;
    int noNodes, id;
    char aux;
    vector<Node *> parkingNodes;
    vector<Node *> realNodes;
    set<Node *> realParkingNodes;

    parkings.open("../Mapa da cidade do Porto-20210505/parking_coordinates.geojson"); //.exe is running from the cmake-build-debug directory
    if (!parkings.is_open()) {
        return 1;
    }
    cout << "File opened\n";
    while (getline(parkings, line)) {
        cout << setprecision(9);
        if (line.find("coordinates") != string::npos) {
            // We are reading a coordinate
            getline(parkings, line);
            iLine = stringstream(line);
            iLine >> longi;
            getline(parkings, line);
            iLine = stringstream(line);
            iLine >> lat;
            parkingNodes.push_back(new Node(longi, lat));
        }
    }
    nodes_lat_long.get(aux);
    cout << "File closed\n";

    nodes_lat_long.open("../Mapa da cidade do Porto-20210505/porto_strong_nodes_latlng.txt");
    if (!nodes_lat_long.is_open()) {
        return 1;
    }
    cout << "File opened\n";
    nodes_lat_long >> noNodes;
    nodes_lat_long.get(aux);
    for (int i = 0; i < noNodes; i++) {
        nodes_lat_long.get(aux);
        nodes_lat_long >> id;
        nodes_lat_long.get(aux);
        nodes_lat_long >> lat;
        nodes_lat_long.get(aux);
        nodes_lat_long >> longi;
        nodes_lat_long.get(aux);
        nodes_lat_long.get(aux);
        realNodes.push_back(new Node(id, lat, longi));
    }

    nodes_lat_long.close();
    cout << "File closed\n";

    for (auto parkingNode: parkingNodes) {
        sort(realNodes.begin(),
             realNodes.end(),
             [parkingNode](const Node *aRealParkingNodeA, const Node *aRealParkingNodeB) {
                 return calcNodeDistance(*aRealParkingNodeA, *parkingNode) < calcNodeDistance(*aRealParkingNodeB, *parkingNode);
             }
        );
        //Closest node to that parking lot
        realParkingNodes.insert(realNodes[0]);
    }

    parkingTags.open("../Mapas-20210505/TagExamples/Porto/t13_tags_porto.txt");
    if (!parkingTags.is_open()) {
        return 1;
    }
    cout << "File opened\n";

    parkingTags << realParkingNodes.size() << '\n';
    parkingTags << "amenity=car_parking\n";
    for (auto realParkingNode: realParkingNodes) {
        parkingTags << realParkingNode->id << '\n';
    }
    parkingTags.close();

    cout << "File closed\n";
}