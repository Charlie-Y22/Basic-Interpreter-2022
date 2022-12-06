/*
 * File: statement.cpp
 * -------------------
 * This file implements the constructor and destructor for
 * the Statement class itself.  Your implementation must do
 * the same for the subclasses you define for each of the
 * BASIC statements.
 */

#include "statement.hpp"

bool if_digit_(const std::string &s) {
    int len = s.length();
    if ((s[0] < '0' or s[0] > '9') and s[0]!='-') return false;
    for (int i = 1; i < len; i++) {
        if (s[i] < '0' or s[i] > '9') return false;
    }
    return true;
}

/* Implementation of the Statement class */

//int stringToInt(std::string str);

Statement::Statement() = default;

Statement::~Statement() = default;

//todo
void Statement::execute(EvalState &state, Program &program) {}

Assignment::Assignment(TokenScanner &scanner) {
    try {
        exp = parseExp(scanner);
    } catch (ErrorException &ex) {
        std::cout << ex.getMessage() << std::endl;
    }
}

Assignment::~Assignment() {
    delete exp;
}

void Assignment::execute(EvalState &state, Program &program) {
    try {
        int value = exp->eval(state);
    } catch (ErrorException &ex) {
        std::cout << ex.getMessage() << std::endl;
    }
}

Print::Print(TokenScanner &scanner) {
    try {
        exp = parseExp(scanner);
    } catch (ErrorException &ex) {
        std::cout << ex.getMessage() << std::endl;
    }
}

Print::~Print() {
    delete exp;
}

void Print::execute(EvalState &state, Program &program) {
    try {
        std::cout << exp->eval(state) << '\n';
    } catch (ErrorException &ex) {
        std::cout << ex.getMessage() << std::endl;
    }
}

Input::Input(TokenScanner &scanner) {
    exp = parseExp(scanner);
}

Input::~Input() {
    delete exp;
}

void Input::execute(EvalState &state, Program &program) {
    while (true) {
        try {
            std::cout << " ? ";
            std::string input;
            getline(std::cin, input);
            if (!if_digit_(input)) error("INVALID NUMBER");
            int value = stringToInteger(input);
            state.setValue(((IdentifierExp *) exp)->getName(), value);
            break;
        } catch (ErrorException &ex) {
            std::cout << ex.getMessage() << std::endl;
        }
    }
}

Goto::Goto(int line_number) : line_num(line_number) {}

Goto::~Goto() = default;

void Goto::execute(EvalState &state, Program &program) {
    program.start_line = line_num;
}

If_then::If_then(TokenScanner &scanner) {
    l.ignoreWhitespace();
    l.scanNumbers();
    r.ignoreWhitespace();
    r.scanNumbers();
    std::string token = scanner.nextToken();
    std::string l_,r_;
    while (token != "<" && token != ">" && token != "=") {
        l_+=token+" ";
        token = scanner.nextToken();
    }
    op = token;
    token = scanner.nextToken();
    while (token != "THEN") {
        r_+=token+" ";
        token = scanner.nextToken();
    }
    std::string line_number = scanner.nextToken();
    if (scanner.hasMoreTokens()) error("SYNTAX ERROR");
    line_num = stringToInteger(line_number);
    l.setInput(l_);
    r.setInput(r_);
    lhs = parseExp(l);
    rhs = parseExp(r);
}

If_then::~If_then() { 
    delete lhs;
    delete rhs;
}

void If_then::execute(EvalState &state, Program &program) {
    if (op == "<") {
        if ((lhs->eval(state)) < (rhs->eval(state))) program.start_line = line_num;
    } else if (op == ">") {
        if ((lhs->eval(state)) > (rhs->eval(state))) program.start_line = line_num;
    } else if (op == "=") {
        if ((lhs->eval(state)) == (rhs->eval(state))) program.start_line = line_num;
    }

}