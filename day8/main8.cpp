#include <iostream>
#include <fstream>
#include <utility>
#include <vector>
#include <regex>

class GameConsole {
public:
    explicit GameConsole(const std::vector<std::string>& lines) : executed(lines.size(), false) {
        code.reserve(lines.size());
        for(const auto& line : lines) {
            code.emplace_back(line);
        }
    }

    // execute next instruction, if available
    // returns whether instruction was executed
    bool step() {
        if(!posValid()) return false;

        // mark instruction as already executed
        executed[position] = true;

        const auto& instr = code[position];
        if(instr.getOp() == Instruction::Op::acc) {
            // change accumulator and go forward
            accumulator += instr.getArg();
            ++position;
        } else if(instr.getOp() == Instruction::Op::jmp) {
            // jump, says Van Halen
            position += instr.getArg();
        } else if(instr.getOp() == Instruction::Op::nop) {
            // do nothing but going one step forward
            ++position;
        }
        return true;
    }

    [[nodiscard]] bool currentInstructionAlreadyExecuted() const {
        if(!posValid()) return false;

        return executed[position];
    }

    [[nodiscard]] int getPosition() const { return position; }
    [[nodiscard]] int getAccumulator() const { return accumulator; }
    [[nodiscard]] bool hasFinished() const { return position >= 0 && static_cast<size_t>(position) >= code.size(); }

    // solve corruption (part two of puzzle) with brute force and return whether it worked
    bool solveCorruption() {
        while(tryNextModification()) {
            do {
                if(currentInstructionAlreadyExecuted()) {
                    std::cout << "x";
                    break;
                }
            } while(step());
            if(hasFinished()) {
                // corruption gone!
                return true;
            }
        }
        return false;
    }

private:
    class Instruction {
    public:
        enum class Op { acc, jmp, nop};
        explicit Instruction(const std::string& s) {
            const std::regex re(R"(^(acc|jmp|nop) ((?:\+|-)[0-9]+$))");
            std::smatch match;
            if(!std::regex_search(s, match, re)) {
                throw std::invalid_argument("not a valid instruction");
            }
            if(match[1] == "acc") {
                op = Op::acc;
            } else if(match[1] == "jmp") {
                op = Op::jmp;
            } else {
                op = Op::nop;
            }
            arg = std::stoi(match[2]);
        }

        [[nodiscard]] Op getOp() const { return op; }
        void setOp(Op newOp) { op = newOp; }
        [[nodiscard]] int getArg() const { return arg; }
    private:
        Op op;
        int arg;
    };

    // whether current position is valid
    [[nodiscard]] bool posValid() const {
        return posValid(position);
    }

    // whether any position is valid
    [[nodiscard]] bool posValid(int pos) const {
        if(pos < 0) return false;
        if(static_cast<size_t>(pos) >= code.size()) return false;
        return true;
    }

    void toggleNopOrJmp(int pos) {
        if(!posValid(pos)) throw std::invalid_argument("invalid pos");

        if(code[pos].getOp() == Instruction::Op::nop) {
            // set nop to jmp
            code[pos].setOp(Instruction::Op::jmp);
        } else if(code[pos].getOp() == Instruction::Op::jmp) {
            // set jmp to nop
            code[pos].setOp(Instruction::Op::nop);
        } else {
            throw std::invalid_argument("not nop or jmp");
        }
    }

    // try the next modification of a nop or jmp instruction
    // returns whether modification was possible
    bool tryNextModification() {
        while(posValid(posNextModification) && code[posNextModification].getOp() == Instruction::Op::acc) {
            ++posNextModification;
        }

        if(posValid(posNextModification)) {
            // modifiable instruction found
            if(posValid(posLastModification)) {
                // reset to original code
                toggleNopOrJmp(posLastModification);
            }

            // change next instruction
            toggleNopOrJmp(posNextModification);
            posLastModification = posNextModification++;

            // reset main values
            position = 0;
            accumulator = 0;
            std::fill(executed.begin(), executed.end(), false); // set all lines to not yet executed

            return true;
        } else {
            // no more instructions to modify available
            return false;
        }
    }

    std::vector<Instruction> code;
    int position = 0; // position in code
    int accumulator = 0; // value of accumulator
    std::deque<bool> executed; // which instructions were already executed

    // values for modifying the source code
    int posLastModification = -1; // position of last modification
    int posNextModification = 0; // which line (if sensible) to modify next
};

int main() {
    std::ifstream infile("../day8/input.txt");
    if(!infile) {
        std::cerr<<"error opening input\n";
        return EXIT_FAILURE;
    }
    std::string line;
    std::vector<std::string> lines;
    while(std::getline(infile, line)) {
        lines.emplace_back(std::move(line));
    }

    GameConsole gc(lines);
    do {
        if(gc.currentInstructionAlreadyExecuted()) {
            std::cout << gc.getPosition() << " is the first instruction to be executed twice\n";
            std::cout << gc.getAccumulator() << " is the current value of the accumulator\n";
            break;
        }
    } while(gc.step());

    std::cout << "----------- Part 2:\n";

    if(gc.solveCorruption()) {
        std::cout << "\nSolution found!\n";
        std::cout << "Accumulator value: "<<gc.getAccumulator()<<"\n";
        return EXIT_SUCCESS;
    } else {
        std::cerr<<"\nNo solution found\n";
        return EXIT_FAILURE;
    }
}