//
// Created by Ali Kooshesh on 4/4/23.
//

#ifndef APYTHONINTERPRETER_ARITHEXPR_HPP
#define APYTHONINTERPRETER_ARITHEXPR_HPP


#include "Token.hpp"
#include "SymTab.hpp"

// Classes in this file define the internal representation of arithmetic expressions.


// An ArithExprNode serves as the base class (super class) for arithmetic expression.
// It forces the derived classes (subclasses) to implement two functions, print and
// evaluate.

class ArithExprNode {
public:
    explicit ArithExprNode(Token token);
    Token token();
    virtual void print() = 0;
    virtual int evaluate(SymTab &symTab) = 0;

private:
    Token _token;

protected:
    bool debug = false;
};


// An InfixExprNode is useful to represent binary arithmetic operators.
class InfixExprNode: public ArithExprNode {  // An expression tree node.

public:
    explicit InfixExprNode(Token tk);

    ArithExprNode *&left();
    ArithExprNode *&right();
    void print () override;
    int evaluate(SymTab &symTab) override;

private:
    ArithExprNode *_left, *_right;
};

// WholeNumber is a leaf-node in an expression tree. It corresponds to
// a terminal in the production rules of the grammar that describes the
// syntax of arithmetic expressions.

class WholeNumber: public ArithExprNode {
public:
    explicit WholeNumber(Token token);
    void print() override;
    int evaluate(SymTab &symTab) override;
};

// Varialbe is a leaf-node in an expression tree. It corresponds to
// a terminal in the production rules of the grammar that describes the
// syntax of arithmetic expressions.

class Variable: public ArithExprNode {
public:
    explicit Variable(Token token);
    void print() override;
    int evaluate(SymTab &symTab) override;
};


#endif //APYTHONINTERPRETER_ARITHEXPR_HPP
