#include <iostream>
#include <fstream>
#include <regex>

int main(int argc, char** argv) {
    (void) argc;
    (void) argv;

    std::ifstream reader("../day2/input.txt");

    if(!reader) {
        std::cerr << "Could not open input file\n";
        return EXIT_FAILURE;
    }

    std::string line;
    std::regex rgx("^([0-9]+?)-([0-9]+?) ([a-z]): ([a-z]+?)$"); // regex to match data in lines
    int numValidPolicy1 = 0;
    int numValidPolicy2 = 0;
    while(std::getline(reader, line)) {
        std::smatch match;
        if(!std::regex_search(line, match, rgx) || match.size() != 4+1) {
            std::cerr << "Issue while parsing line\n";
            return EXIT_FAILURE;
        }

        const size_t num1 = std::stoi(match[1]);
        const size_t num2 = std::stoi(match[2]);
        const std::string& c = match[3];
        const std::string& password = match[4];

        // check with policy 1
        const size_t occurrence = std::count(password.begin(), password.end(), c[0]);
        if(occurrence < num1 || occurrence > num2) {
            std::cout << "policy 1 invalid: " << line << "\n";
        } else {
            std::cout << "policy 1 valid: " << line << "\n";
            ++numValidPolicy1;
        }

        // check with policy 2
        size_t letterMatch = 0;
        if(password.size() > num1-1 && password[num1-1] == c[0]) {
            ++letterMatch;
        }
        if(password.size() > num2-1 && password[num2-1] == c[0]) {
            ++letterMatch;
        }
        if(letterMatch != 1) {
            std::cout << "policy 2 invalid: " << line << "\n";
        } else {
            std::cout << "policy 2 valid: " << line << "\n";
            ++numValidPolicy2;
        }

    }

    std::cout << numValidPolicy1 << " passwords are valid with policy 1\n";
    std::cout << numValidPolicy2 << " passwords are valid with policy 2\n";

    return EXIT_SUCCESS;
}