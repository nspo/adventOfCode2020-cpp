#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <regex>

using ContainedBagsType = std::vector<std::pair<int,std::string>>;
using RulesType = std::map<std::string, ContainedBagsType>;

void stringToRule(const std::string& s, RulesType& rules) {
    // identify type of outer bag for rule
    std::regex reOuterBag("^([a-z ]+?) bags contain");
    std::smatch match;
    if(!std::regex_search(s, match, reOuterBag)) throw std::invalid_argument("could not parse (outer bag)");
    const std::string& outerBag = match[1];

    std::regex reSubBags("([0-9]+) ([a-z ]+) (?:bag|bags)");
    auto itStrStart = s.cbegin();
    while(std::regex_search(itStrStart, s.cend(), match, reSubBags)) {
        // sub bag rule found
//        std::cout << "Found sub-bag rule for "<<outerBag<<": "<<match[1]<<"x"<<match[2]<<"\n";
        itStrStart = match.suffix().first;

        // fill in actual rule
        rules[outerBag].emplace_back(std::make_pair(std::stoi(match[1]), match[2]));
    }
}

bool containsAtSomeLevel(const std::string& outerBag, const std::string& innerBag, RulesType& rules, std::map<std::string,bool>& cache) {
    if(auto it = cache.find(outerBag); it != cache.end()) {
        return it->second;
    }

    if(rules[outerBag].empty()) {
        cache[outerBag] = false;
        return false;
    }
    for(const auto& containedBag : rules[outerBag]) {
        if(containedBag.second == innerBag) {
            cache[outerBag] = true;
            return true;
        }
    }
    // does not directly contain a innerBag of correct color
    // check sub-bags (could be cached...)
    for(const auto& containedBag : rules[outerBag]) {
        if(containsAtSomeLevel(containedBag.second, innerBag, rules, cache)) {
            cache[outerBag] = true;
            return true;
        }
    }

    cache[outerBag] = false;
    return false;
}

int countSubBags(const std::string& bagName, RulesType& rules, std::map<std::string,int>& cache) {
    if(auto it = cache.find(bagName); it != cache.end()) {
        return it->second;
    }

    if(rules[bagName].empty()) {
        cache[bagName] = 0;
        return 0;
    }

    int subBags = 0;
    for(const auto& rule : rules[bagName]) {
        subBags += rule.first * (1 + countSubBags(rule.second, rules, cache));
    }

    cache[bagName] = subBags;
    return subBags;
}

int main(int argc, char** argv) {
    (void) argc;
    (void) argv;

    std::ifstream infile("../day7/input.txt");
    if(!infile) {
        std::cerr<<"Error while opening input file\n";
        return EXIT_FAILURE;
    }

    std::string line;
    RulesType rules;
    while(std::getline(infile, line)) {
        stringToRule(line, rules);
    }

    // how many colors contain >= 1 shiny gold bag?
    int numShinyGold = 0;
    std::map<std::string,bool> cache{}; // cache for lookup
    for(const auto& rule : rules) {
        if(containsAtSomeLevel(rule.first, "shiny gold", rules, cache)) {
            ++numShinyGold;
        }
    }
    std::cout<<numShinyGold<<" bags contain >= 1 shiny gold bag at some level\n";

    std::map<std::string,int> cache2;
    std::cout<<countSubBags("shiny gold", rules, cache2)<< " sub-bags in total in shiny gold bag\n";
    return EXIT_SUCCESS;
}