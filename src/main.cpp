#include <iostream>
#include "Algorithm.h"

int main() {
    std::cout << "Hello, World!" << std::endl;
    Algorithm("../Mapas-20210505/PortugalMaps/Porto/nodes_lat_lon_porto.txt",
              "../Mapas-20210505/PortugalMaps/Porto/edges_porto.txt",
              "../Mapas-20210505/TagExamples/Porto/t12_tags_porto.txt");
    return 0;
}
