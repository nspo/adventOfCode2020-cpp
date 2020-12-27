#include <iostream>
#include <fstream>
#include <map>
#include <set>
#include <vector>
#include <algorithm>

void solveWithPolicy1() {
    std::ifstream reader("../day6/input.txt");
    if(!reader) {
        std::cerr<<"Error while opening input\n";
        return;
    }

    std::string line;
    std::set<char> yesSet;
    int totalYesCount = 0;
    while(std::getline(reader, line)) {
        if(line.empty()) {
            // a group is finished
            totalYesCount += yesSet.size();
            yesSet.clear();
        } else {
            for(const char c : line) {
                yesSet.insert(c);
            }
        }
    }
    // last group read
    totalYesCount += yesSet.size();

    std::cout << "Total yes count over groups (policy 1): "<<totalYesCount<<"\n";
}

// intersection of multiple sets
template<typename T>
std::set<T> multi_intersect(const std::vector<std::set<T>>& sets) {
    if(sets.empty()) return {};

    // save each step in alternating set
    std::set<T> tmp1 = sets[0];
    std::set<T> tmp2;
    for(size_t i=1; i<sets.size(); ++i) {
        if(i%2 == 0) {
            tmp1.clear();
            std::set_intersection(tmp2.begin(), tmp2.end(), sets[i].begin(), sets[i].end(), std::inserter(tmp1, tmp1.begin()));
        } else {
            tmp2.clear();
            std::set_intersection(tmp1.begin(), tmp1.end(), sets[i].begin(), sets[i].end(), std::inserter(tmp2, tmp2.begin()));
        }
    }

    if (sets.size() % 2 == 0) {
        return tmp2;
    } else {
        return tmp1;
    }
}

void solveWithPolicy2() {
    std::ifstream reader("../day6/input.txt");
    if(!reader) {
        std::cerr<<"Error while opening input\n";
        return;
    }

    int totalYesCount = 0;
    std::string line;
    std::vector<std::set<char>> yesSets;
    while(std::getline(reader, line)) {
        if(line.empty()) {
            // one group finished
            std::set<char> commonYes = multi_intersect(yesSets);
            totalYesCount += commonYes.size();
            yesSets.clear();
        } else {
            std::set<char> yesSet;
            for(const char c : line) {
                yesSet.insert(c);
            }
            yesSets.emplace_back(std::move(yesSet));
        }
    }
    // last group finished
    std::set<char> commonYes = multi_intersect(yesSets);
    totalYesCount += commonYes.size();

    std::cout << "Total yes count over groups (policy 2): "<<totalYesCount<<"\n";
}

int main(int argc, char** argv) {
    (void) argc;
    (void) argv;

    solveWithPolicy1();
    solveWithPolicy2();
    return 0;
}