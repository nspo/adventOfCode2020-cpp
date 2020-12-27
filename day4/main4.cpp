#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <regex>

bool passport_valid1(const std::map<std::string,std::string>& passport) {
    const std::vector<std::string> neededKeys = {"byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid"};

    return std::all_of(neededKeys.begin(), neededKeys.end(),
                [&passport](const std::string& key) { return passport.find(key) != passport.end(); });
}

bool passport_valid2(std::map<std::string,std::string>& passport) {
    std::smatch match;

    // byr
    {
        std::regex re("^([0-9]{4})$");
        if(!std::regex_search(passport["byr"], match, re)) return false;
        int byr = std::stoi(match[1]);
        if(byr < 1920 || byr > 2002)  return false;

    }

    // iyr
    {
        std::regex re("^([0-9]{4})$");
        if(!std::regex_search(passport["iyr"], match, re)) return false;
        int iyr = std::stoi(match[1]);
        if(iyr < 2010 || iyr > 2020) return false;

    }

    // eyr
    {
        std::regex re("^([0-9]{4})$");
        if(!std::regex_search(passport["eyr"], match, re)) return false;
        int eyr = std::stoi(match[1]);
        if(eyr < 2020 || eyr > 2030) return false;

    }

    // hgt
    {
        std::regex re("^([0-9]{2,3})(in|cm)$");
        if(!std::regex_search(passport["hgt"], match, re)) return false;

        int hgt = std::stoi(match[1]);
        std::string unit = match[2];
        if(unit == "cm") {
            if(hgt < 150 || hgt > 193) return false;
        } else if(unit == "in") {
            if(hgt < 59 || hgt > 76) return false;
        } else {
            return false;
        }
    }

    // hcl
    {
        std::regex re("^#[0-9a-f]{6}$");
        if(!std::regex_search(passport["hcl"], match, re)) return false;
    }

    // ecl
    {
        std::regex re("^(amb|blu|brn|gry|grn|hzl|oth)$"); // yes, this could be done without regex... but we've already set it up
        if(!std::regex_search(passport["ecl"], match, re)) return false;
    }

    // pid
    {
        std::regex re("^[0-9]{9}$");
        if(!std::regex_search(passport["pid"], match, re)) return false;
    }

    return true;
}

int main(int argc, char** argv) {
    (void) argc;
    (void) argv;

    std::ifstream reader("../day4/input.txt");
    if(!reader) {
        std::cerr << "Error while opening input\n";
        return EXIT_FAILURE;
    }

    std::string line;
    std::vector<std::map<std::string,std::string>> passports;
    std::map<std::string,std::string> passport;
    while(getline(reader, line)) {
        if(line.empty()) {
            // person finished
            passports.emplace_back(std::move(passport));
            passport.clear();
        } else {
            // read data into passport var
            const std::regex re(R"(([a-z]{3}):(.+?)(?:\s|$))");
            std::smatch match;
            auto itStart = line.cbegin();
            while(std::regex_search(itStart, line.cend(), match, re) && match.size() == 2+1) {
                // matched a key-value pair
                passport[match[1]] = match[2];

                itStart = match.suffix().first;
            }

        }
    }
    // last person finished -> also save it
    passports.emplace_back(std::move(passport));

    int numValid1 = std::count_if(passports.begin(), passports.end(), passport_valid1);
    std::cout<<numValid1<<" passports are valid (policy 1)\n";

    int numValid2 = std::count_if(passports.begin(), passports.end(), passport_valid2);
    std::cout<<numValid2<<" passports are valid (policy 2)\n";

    return 0;
}