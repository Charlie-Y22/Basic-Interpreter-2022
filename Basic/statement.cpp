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
    for (int i = 0; i < len; i++) {
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
    exp = parseExp(scanner);
}

Assignment::~Assignment() {
    delete exp;
}

void Assignment::execute(EvalState &state, Program &program) {
    int value = exp->eval(state);
}

Print::Print(TokenScanner &scanner) {
    exp = parseExp(scanner);
}

Print::~Print() {
    delete exp;
}

void Print::execute(EvalState &state, Program &program) {
    std::cout << exp->eval(state) << '\n';
}

Input::Input(TokenScanner &scanner) {
    exp = parseExp(scanner);
}

Input::~Input() {
    delete exp;
}

void Input::execute(EvalState &state, Program &program) {
    std::cout << '?';
    std::string input;
    getline(std::cin, input);
    while (!if_digit_(input)) {
        std::cout << "INVALID NUMBER\n";
        std::cout << '?';
        getline(std::cin, input);
    }
    int value = stringToInteger(input);
    state.setValue(((IdentifierExp *) exp)->getName(), value);
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
    while (token != "<" && token != ">" && token != "=") {
        l.saveToken(token);
        token = scanner.nextToken();
    }
    op = token;
    token = scanner.nextToken();
    while (token != "THEN") {
        r.saveToken(token);
        token = scanner.nextToken();
    }
    std::string line_number = scanner.nextToken();
    if (scanner.hasMoreTokens()) error("SYNTAX ERROR");
    line_num = stringToInteger(line_number);
}

If_then::~If_then()=default;

void If_then::execute(EvalState &state, Program &program) {
    if (op == "<") {
        if ((parseExp(l)->eval(state)) >= (parseExp(r)->eval(state))) return;
    } else if (op == ">") {
        if ((parseExp(l)->eval(state)) <= (parseExp(r)->eval(state))) return;
    } else if (op == "=") {
        if ((parseExp(l)->eval(state)) != (parseExp(r)->eval(state))) return;
    }
    program.start_line = line_num;
}