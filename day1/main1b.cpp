#include <iostream>
#include <fstream>
#include <set>
#include <vector>
#include <algorithm>

// find the three values which sum up to 2020 and multiply them

// solve with non-brute-force method
std::optional<int> better_method(const std::vector<int>& vec) {
    for(size_t i=0; i<vec.size()-2; ++i) {
        if(i > 0 && vec[i-1] == vec[i]) {
            // skip;
            continue;
        }

        size_t start = i+1;
        size_t end = vec.size()-1;
        while(start < end) {
            const int& a = vec[i];
            const int& b = vec[start];
            const int& c = vec[end];
            int sum = a+b+c;
            if(sum < 2020) {
                ++start;
            } else if(sum > 2020) {
                --end;
            } else {
                // match!
                std::cout<<a<<", "<<b<<", and "<<c<<" sum up to 2020!\n";
                std::cout<<"Their product is "<<a*b*c<<"\n";
                return {a*b*c};
            }
        }
    }

    return {};
}

// solve with brute force
std::optional<int> brute_force(const std::vector<int>& vec) {
    for(size_t i=0; i<vec.size()-2; ++i) {
        for(size_t j=1; j<vec.size()-1; ++j) {
            for(size_t k=2; k<vec.size(); ++k) {
                int res = vec[i]+vec[j]+vec[k];
                if(res == 2020) {
                    return {vec[i]*vec[j]*vec[k]};
                }
            }
        }
    }
    return {};
}

int main() {
    std::ifstream reader("../day1/input.txt");
    if(!reader) {
        std::cerr << "Error while opening input file\n";
        return EXIT_FAILURE;
    }

    // read in as set
    std::set<int> valuesSet;
    int value;
    while(reader >> value) {
        valuesSet.insert(value);
    }

    // insert ordered set values into vector (could also read in as vec and sort later of course)
    std::vector<int> valuesVec(valuesSet.begin(), valuesSet.end());

    // find solution with better method
    std::optional<int> other_solution = better_method(valuesVec);
    if(!other_solution) {
        std::cerr << "No solution found with better method\n";
    } else {
        std::cout << "Better method solution: "<<*other_solution<<"\n";
    }

    std::cout << "---\n";

    std::cout << "Now using brute force...\n";

    // find brute force solution
    std::optional<int> brute_force_solution = brute_force(valuesVec);
    if(!brute_force_solution) {
        std::cerr << "No solution found with brute force\n";
    } else {
        std::cout << "Brute force solution: "<<*brute_force_solution<<"\n";
    }

    return EXIT_SUCCESS;
}
