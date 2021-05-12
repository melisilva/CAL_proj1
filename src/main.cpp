#include <iostream>
#include "stdlib.h"
#include "time.h"
#include "Algorithm.h"
#include "Interface.h"



int main() {
    srand(1); //DEBUGs
    std::cout << "Hello, World!" << std::endl;
    Algorithm algo("../Mapas-20210505/PortugalMaps/Porto/nodes_lat_lon_porto.txt",
              "../Mapas-20210505/PortugalMaps/Porto/edges_porto.txt",
              "../Mapas-20210505/TagExamples/Porto/t12_tags_porto.txt");

    Interface<int> interface(algo);
    interface.execute();
    return 0;
}
