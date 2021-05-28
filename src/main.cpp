#include <iostream>
#include "stdlib.h"
#include "time.h"
#include "Algorithm.h"
#include "Interface.h"


int main() {
    srand(time(NULL)); 
    std::cout << "Hello, World!" << std::endl;
    Algorithm algo("../Mapa da cidade do Porto-20210505/porto_strong_nodes_latlng.txt",
                   "../Mapa da cidade do Porto-20210505/porto_strong_edges.txt",
                   "../Parking Tags/t13_tags_porto.txt");

    Interface<int> interface(algo);
    interface.execute();
    return 0;
}
