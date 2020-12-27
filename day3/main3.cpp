#include <cstdlib>
#include <fstream>
#include <vector>
#include <iostream>
#include <array>

// return value at map coordinates
char byIdx(const size_t horz, const size_t vert, const std::vector<std::string>& map) {
    if(vert >= map.size() || map[0].empty()) {
        throw std::invalid_argument("map size issue");
    }

    size_t actualHorz = horz % map[0].size();

    return map[vert][actualHorz];
}

int countTrees(const size_t horzStepSize, const size_t vertStepSize, const std::vector<std::string>& map) {
    int numTrees = 0;
    for(size_t vert=0, horz=0; vert<map.size(); vert+=vertStepSize, horz+=horzStepSize) {
        // go from top to bottom
        if(byIdx(horz, vert, map) == '#') {
            ++numTrees;
        }
    }

    return numTrees;
}



int main(int argc, char** argv) {
    (void) argc;
    (void) argv;

    std::ifstream reader("../day3/input.txt");
    if(!reader) {
        std::cerr<<"Error while opening file.\n";
        return EXIT_FAILURE;
    }

    std::vector<std::string> map{};
    std::string line;
    while(getline(reader, line)) {
        map.push_back(line);
    }


    const std::vector<std::pair<int,int>> stepSizeVec = {
            {1,1},
            {3,1},
            {5,1},
            {7,1},
            {1,2},
    };

    const std::vector<std::pair<int,int>> bounds = {{1,2}, {2,7}, {3,4}};

    long product = 1;
    for(const auto& stepSizes : stepSizeVec) {
        int numTrees = countTrees(stepSizes.first, stepSizes.second, map);
        std::cout<<"right "<<stepSizes.first<<", down "<<stepSizes.second<<": "<<numTrees<<" trees\n";
        product *= numTrees;
    }

    std::cout<<"Product: "<<product<<"\n";

    return EXIT_SUCCESS;
}