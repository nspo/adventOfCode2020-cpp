#include <iostream>
#include <fstream>
#include <set>

int main() {
    std::ifstream reader("../day1/input.txt");
    if(!reader) {
        std::cerr << "Error while opening input file\n";
        return EXIT_FAILURE;
    }

    // solution with set - other solutions see other cpp file

    std::set<int> seenValues;

    int value;
    while(reader >> value) {
        std::cout<<"read "<<value<<"\n";
        int needed = 2020-value;
        if(seenValues.find(needed) != seenValues.end()) {
            std::cout << "Found the needed value "<<needed<<"!\n";
            std::cout << "Product of "<<value<<" and "<<needed<<" is "<<value*needed<<"\n";
            return EXIT_SUCCESS;
        }
        seenValues.insert(value);
    }
    return 0;
}
