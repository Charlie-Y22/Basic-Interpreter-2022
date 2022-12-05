/*
 * File: Basic.cpp
 * ---------------
 * This file is the starter project for the BASIC interpreter.
 */

#include <cctype>
#include <iostream>
#include <string>
#include "exp.hpp"
#include "parser.hpp"
#include "program.hpp"
#include "Utils/error.hpp"
#include "Utils/tokenScanner.hpp"
#include "Utils/strlib.hpp"


/* Function prototypes */

void processLine(std::string line, Program &program, EvalState &state);

/* Main program */

int main() {
    EvalState state;
    Program program;
    //cout << "Stub implementation of BASIC" << endl;
    while (true) {
        try {
            std::string input;
            getline(std::cin, input);
            if (input.empty())
                return 0;
            processLine(input, program, state);
        } catch (ErrorException &ex) {
            std::cout << ex.getMessage() << std::endl;
        }
    }
    return 0;
}

bool if_digit(const std::string &s) {
    int len = s.length();
    for (int i = 0; i < len; i++) {
        if (s[i] < '0' or s[i] > '9') return false;
    }
    return true;
}

/*
 * Function: processLine
 * Usage: processLine(line, program, state);
 * -----------------------------------------
 * Processes a single line entered by the user.  In this version of
 * implementation, the program reads a line, parses it as an expression,
 * and then prints the result.  In your implementation, you will
 * need to replace this method with one that can respond correctly
 * when the user enters a program line (which begins with a number)
 * or one of the BASIC commands, such as LIST or RUN.
 */



void processLine(std::string line, Program &program, EvalState &state) {
    TokenScanner scanner;
    scanner.ignoreWhitespace();
    scanner.scanNumbers();
    scanner.setInput(line);
//    Expression *exp = parseExp(scanner);
//    int value = exp->eval(state);
//    std::cout << value << std::endl;
//    delete exp;
    std::string begin = scanner.nextToken();
    TokenType type = scanner.getTokenType(begin);
    if (type != NUMBER) {
        if (!scanner.hasMoreTokens()) {
            if (begin == "RUN") {
                program.start(state);
                return;
            } else if (begin == "LIST") {
                for (auto iter = program.order.begin(); iter != program.order.end(); iter++) {
                    std::cout << program.lines[*iter] << '\n';
                }
                return;
            } else if (begin == "CLEAR") {
                program.clear();
                state.Clear();
            } else if (begin == "QUIT" or begin == "END") {
                program.clear();
                state.Clear();
                exit(0);
            } else if (begin == "HELP") {
                std::cout << "This is a Minimal BASIC Interpreter." << std::endl;
                return;
            } else error("SYNTAX ERROR");
        }
        if (begin == "LET") {
            Expression *exp = parseExp(scanner);
            int value = exp->eval(state);
            delete exp;
        }
        if (begin == "PRINT") {
            Expression *exp = parseExp(scanner);
            std::cout << exp->eval(state) << '\n';
            delete exp;
        }
        if (begin == "INPUT") {
            Expression *exp = parseExp(scanner);
            std::cout << '?';
            std::string input;
            getline(std::cin, input);
            while (!if_digit(input)) {
                std::cout << "INVALID NUMBER\n";
                std::cout << '?';
                getline(std::cin, input);
            }
            int value = stringToInteger(input);
            state.setValue(((IdentifierExp *) exp)->getName(), value);
            delete exp;
        }
    }
    if (type == NUMBER) {
        int line_num = stringToInteger(begin);
        if (!scanner.hasMoreTokens()) {
            program.removeSourceLine(line_num);
        } else {
            program.addSourceLine(line_num, line);
        }
    }
    // todo


    // end
}






