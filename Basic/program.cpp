/*
 * File: program.cpp
 * -----------------
 * This file is a stub implementation of the program.h interface
 * in which none of the methods do anything beyond returning a
 * value of the correct type.  Your job is to fill in the bodies
 * of each of these methods with an implementation that satisfies
 * the performance guarantees specified in the assignment.
 */

#include "program.hpp"


Program::Program() = default;

Program::~Program() = default;

void Program::clear() {
    // Replace this stub with your own code
    //todo
    lines.clear();
    for (auto iter = order.begin(); iter != order.end(); iter++) {
        Statement *state_ = getParsedStatement(*iter);
        delete state_;
    }
    parse.clear();
    order.clear();
}

void Program::addSourceLine(int lineNumber, const std::string &line) {
    // Replace this stub with your own code
    //todo
    if (parse.count(lineNumber)) {
        delete parse[lineNumber];
        parse.erase(lineNumber);
    } else if (!lines.count(lineNumber)) order.insert(lineNumber);
    lines[lineNumber] = line;
}

void Program::removeSourceLine(int lineNumber) {
    // Replace this stub with your own code
    //todo
    if (!lines.count(lineNumber)) return;
    else if (parse.count(lineNumber)) {
        delete parse[lineNumber];
        parse.erase(lineNumber);
    }
    lines.erase(lineNumber);
    order.erase(lineNumber);

}

std::string Program::getSourceLine(int lineNumber) {
    // Replace this stub with your own code
    //todo
    if (!lines.count(lineNumber)) return "";
    else return lines[lineNumber];
}

void Program::setParsedStatement(int lineNumber, Statement *stmt) {
    // Replace this stub with your own code
    //todo
    if (!lines.count(lineNumber)) error("SYNTAX ERROR");
    else if (parse.count(lineNumber)) {
        delete parse[lineNumber];
    }
    parse[lineNumber] = stmt;
}

Statement *Program::getParsedStatement(int lineNumber) {
    // Replace this stub with your own code
    //todo
    if (!parse.count(lineNumber)) return nullptr;
    else return parse[lineNumber];
}

int Program::getFirstLineNumber() {
    // Replace this stub with your own code
    //todo
    if (order.empty()) return -1;
    else return *order.begin();
}

int Program::getNextLineNumber(int lineNumber) {
    // Replace this stub with your own code
    //todo
    auto iter = order.find(lineNumber);
    ++iter;
    if (iter == order.end()) return -1;
    else return *iter;
}

//more func to add
//todo
void Program::start(EvalState &state) {
    start_line = getFirstLineNumber();
    while (start_line != -1) {
        TokenScanner scanner;
        scanner.ignoreWhitespace();
        scanner.scanNumbers();
        scanner.setInput(lines[start_line]);
        std::string instruction = scanner.nextToken();
        instruction = scanner.nextToken();
        if (instruction == "REM") {
            start_line = getNextLineNumber(start_line);
        } else if (instruction == "END") {
            start_line=-1;
        } else if (instruction == "LET") {
            Statement *stat = new Assignment(scanner);
            stat->execute(state, *this);
            setParsedStatement(start_line, stat);
            start_line = getNextLineNumber(start_line);
        } else if (instruction == "PRINT") {
            Statement *stat = new Print(scanner);
            stat->execute(state, *this);
            setParsedStatement(start_line, stat);
            start_line = getNextLineNumber(start_line);
        } else if (instruction == "INPUT") {
            Statement *stat = new Input(scanner);
            stat->execute(state, *this);
            setParsedStatement(start_line, stat);
            start_line = getNextLineNumber(start_line);
        } else if (instruction == "GOTO") {
            std::string line_ = scanner.nextToken();
            int tmp = start_line;
            int next_line = stringToInteger(line_);
            Statement *stat = new Goto(next_line);
            stat->execute(state, *this);
            try {
                if (!lines.count(start_line)) error("LINE NUMBER ERROR");
                setParsedStatement(tmp, stat);
            } catch (ErrorException &ex) {
                start_line = -1;
                delete stat;
                std::cout << ex.getMessage() << std::endl;
            }
        } else if (instruction == "IF") {
            int tmp = start_line;
            Statement *stat = new If_then(scanner);
            start_line = getNextLineNumber(start_line);
            stat->execute(state, *this);
            try {
                if (!lines.count(start_line)) error("LINE NUMBER ERROR");
                setParsedStatement(tmp, stat);
            } catch (ErrorException &ex) {
                start_line = -1;
                delete stat;
                std::cout << ex.getMessage() << std::endl;
            }
        }
    }
}
