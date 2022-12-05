/*
 * File: statement.h
 * -----------------
 * This file defines the Statement abstract type.  In
 * the finished version, this file will also specify subclasses
 * for each of the statement types.  As you design your own
 * version of this class, you should pay careful attention to
 * the exp.h interface, which is an excellent model for
 * the Statement class hierarchy.
 */

#ifndef _statement_h
#define _statement_h

#include <string>
#include <sstream>
#include "evalstate.hpp"
#include "exp.hpp"
#include "Utils/tokenScanner.hpp"
#include "program.hpp"
#include "parser.hpp"
#include "Utils/error.hpp"
#include "Utils/strlib.hpp"

class Program;

/*
 * Class: Statement
 * ----------------
 * This class is used to represent a statement in a program.
 * The model for this class is Expression in the exp.h interface.
 * Like Expression, Statement is an abstract class with subclasses
 * for each of the statement and command types required for the
 * BASIC interpreter.
 */

class Statement {

public:

/*
 * Constructor: Statement
 * ----------------------
 * The base class constructor is empty.  Each subclass must provide
 * its own constructor.
 */

    Statement();

/*
 * Destructor: ~Statement
 * Usage: delete stmt;
 * -------------------
 * The destructor deallocates the storage for this expression.
 * It must be declared virtual to ensure that the correct subclass
 * destructor is called when deleting a statement.
 */

    virtual ~Statement();

/*
 * Method: execute
 * Usage: stmt->execute(state);
 * ----------------------------
 * This method executes a BASIC statement.  Each of the subclasses
 * defines its own execute method that implements the necessary
 * operations.  As was true for the expression evaluator, this
 * method takes an EvalState object for looking up variables or
 * controlling the operation of the interpreter.
 */

    virtual void execute(EvalState &state, Program &program);

};


/*
 * The remainder of this file must consists of subclass
 * definitions for the individual statement forms.  Each of
 * those subclasses must define a constructor that parses a
 * statement from a scanner and a method called execute,
 * which executes that statement.  If the private data for
 * a subclass includes data allocated on the heap (such as
 * an Expression object), the class implementation must also
 * specify its own destructor method to free that memory.
 */

// todo
class Assignment : public Statement { // LET
public:
    Assignment(TokenScanner &scanner);

    virtual ~Assignment();

    virtual void execute(EvalState &state, Program &program);

private:
    Expression *exp;
};

class Print : public Statement { // PRINT
public:
    Print(TokenScanner &scanner);

    virtual ~Print();

    virtual void execute(EvalState &state, Program &program);

private:
    Expression *exp;
};

class Input : public Statement { // INPUT
public:
    Input(TokenScanner &scanner);

    virtual ~Input();

    virtual void execute(EvalState &state, Program &program);

private:
    Expression *exp;
};

class Goto : public Statement { // GOTO
public:
    Goto(int line_number);

    virtual ~Goto();

    virtual void execute(EvalState &state, Program &program);
private:
    int line_num;
};

class If_then : public Statement { // IF THEN
public:
    If_then(TokenScanner &scanner);

    virtual  ~If_then();

    virtual void execute(EvalState &state, Program &program);

private:
    TokenScanner l;
    TokenScanner r;
//    Expression *lhs;
//    Expression *rhs;
    std::string op;
    int line_num;
};
#endif
