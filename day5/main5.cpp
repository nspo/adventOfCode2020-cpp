#include <iostream>
#include <fstream>
#include <cassert>
#include <vector>
#include <algorithm>
#include <deque>

// helper class for seat identification
class BinarySelector {
public:
    BinarySelector(int min, int max) {
        assert(min <= max);
        this->min = min;
        this->max = max;
    }

    // constrict to upper half
    void top() {
        min = (min+max)/2+1;
    }

    // constrict to lower half
    void bottom() {
        max = (min+max)/2;
    }

    [[nodiscard]] int get() const {
        if(min != max) throw std::logic_error("Did not find element yet");
        return min;
    }

private:
    int min;
    int max;
};

class BoardingPass {
public:
    int row;
    int col;
    int id;

    // init with 10-letter code
    explicit BoardingPass(const std::string& code) {
        if(code.size() != 10) throw std::invalid_argument("code length must be 10");

        // row identification
        BinarySelector rowSelector(0, 127);
        for(int i=0; i<7; ++i) {
            if(code[i] == 'F') {
                rowSelector.bottom();
            } else if(code[i] == 'B') {
                rowSelector.top();
            } else {
                throw std::invalid_argument("code malformed");
            }
        }
        this->row = rowSelector.get();

        // column identification
        BinarySelector colSelector(0, 7);
        for(int i=7; i<10; ++i) {
            if(code[i] == 'L') {
                colSelector.bottom();
            } else if(code[i] == 'R') {
                colSelector.top();
            } else {
                throw std::invalid_argument("code malformed");
            }
        }
        this->col = colSelector.get();
        this->id = this->row*8 + this->col;
    }
};

int main(int argc, char** argv) {
    (void) argc;
    (void) argv;

    std::ifstream reader("../day5/input.txt");
    if(!reader) {
        std::cerr << "Error while opening input\n";
        return EXIT_FAILURE;
    }

    std::vector<BoardingPass> bps;
    std::string line;
    std::deque<bool> seatTaken(128*8, false); // don't use vector<bool>
    while(std::getline(reader, line)) {
        bps.emplace_back(line);
        seatTaken[(--bps.end())->id] = true;
    }

    auto itHighestId = std::max_element(bps.begin(), bps.end(), [](const BoardingPass& bp1, const BoardingPass& bp2) { return bp1.id < bp2.id; });
    std::cout<<"highest seat id: "<<itHighestId->id<<"\n";

    // find seat that's not taken but has rows with people in front and behind it
    for(int id=0+1; id<128*8-1; ++id) {
        if(!seatTaken[id] && seatTaken[id-1] && seatTaken[id+1]) {
            std::cout<<id<<" is your seat\n";
            break;
        }
    }
}