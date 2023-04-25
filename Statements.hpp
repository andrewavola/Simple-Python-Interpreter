//
// Created by Ali Kooshesh on 4/4/23.
//

#ifndef APYTHONINTERPRETER_STATEMENTS_HPP
#define APYTHONINTERPRETER_STATEMENTS_HPP

#include <iostream>
#include <vector>

#include "ExprNode.hpp"
#include "SymTab.hpp"

// The Statement (abstract) class serves as a super class for all statements that
// are defined in the language. Ultimately, statements have to be evaluated.
// Therefore, this class defines evaluate, a pure-virtual function to make
// sure that all subclasses of Statement provide an implementation for this
// function.

class Statement {
public:
    Statement();

    virtual void print() = 0;
    virtual void evaluate(SymTab &symTab) = 0;

};


// Statements is a collection of Statement. For example, all statements in a function
// can be represented by an instance of Statements.

class Statements {
public:
    Statements();

    void addStatement(Statement *statement);
    void evaluate(SymTab &symTab);

    void print();

private:
    std::vector<Statement *> _statements;
};

// AssignmentStatement represents the notion of an lValue having been assigned an rValue.
// The rValue is an expression.

class AssignmentStatement : public Statement {
public:
    AssignmentStatement();
    AssignmentStatement(std::string lhsVar, ExprNode *rhsExpr);

    std::string &lhsVariable();
    ExprNode *&rhsExpression();

    virtual void evaluate(SymTab &symTab);
    virtual void print();

private:
    std::string _lhsVariable;
    ExprNode *_rhsExpression;
};

// PrintStatement represents printing the value of whichever ID is to the right
// of the "print" keyword by reaching into the symbol table map and grabbing the
// current mapped value.
class PrintStatement : public Statement {
public:
    //constructors
    PrintStatement();
    PrintStatement(std::vector<ExprNode *> expressions);

    std::vector<ExprNode *> &getExpressions();
    virtual void evaluate(SymTab &symTab);
    virtual void print();

private:
    std::vector<ExprNode *> _relExpressions;
    // ExprNode *_rhsExpression;
    //std::string _rhsID;
};

//ForStatement
class ForStatement : public Statement{
public: 
    //constructors
    ForStatement();
    ForStatement(AssignmentStatement *leftOfSemiColon, ExprNode *mid, AssignmentStatement *rightOfSemiColon, std::vector<Statements *> stmtVec);
    
    //Function
    void addToForLoopStatements(Statements *stmts);

    //Getters
    AssignmentStatement *&left();
    AssignmentStatement *&right();
    ExprNode *&mid();
    std::vector<Statements *> returnVec();

    //Evaluations
    virtual void evaluate(SymTab &symTab);
    virtual void print();

private:
    AssignmentStatement *_losc;
    ExprNode *_middle;
    AssignmentStatement *_rosc;
    std::vector<Statements *> _forLoopStatements;

};
#endif //APYTHONINTERPRETER_STATEMENTS_HPP
