//
// Created by Ali Kooshesh on 4/4/23.
//

#ifndef APYTHONINTERPRETER_STATEMENTS_HPP
#define APYTHONINTERPRETER_STATEMENTS_HPP

#include <iostream>
#include <vector>

#include "ExprNode.hpp"
#include "SymTab.hpp"
#include "Range.hpp"

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
    ForStatement(Statements *stmtVec, Range* rng);
    
    //Function
    void addToForLoopStatements(Statements *stmts);

    //Getters
    Statements *returnVec(){return _forLoopStatements;}
    Range *returnRng(){return _rng;}

    //Evaluations
    virtual void evaluate(SymTab &symTab);
    virtual void print();

private:
    Statements *_forLoopStatements;
    Range *_rng;

};

class IfStatement : public Statement{
public:
    //Constructors
    IfStatement();
    IfStatement(Statements *ifVec, ExprNode *ifConditional, std::vector<ExprNode *> elifConditions
                , std::vector<Statements *> elVec, Statements *elseStmt);

    //Getters
    Statements *returnIfVec(){return ifVec;}
    std::vector<Statements *> returnElifVec(){return elifVec;}
    Statements *returnElseVec(){return elseVec;}
    ExprNode *returnCondition(){return cond;}
    

    //Abstract class functions
    virtual void evaluate(SymTab &symTab);
    virtual void print();

private:
    bool elseExists = false;
    Statements *ifVec;
    ExprNode *cond;
    std::vector<ExprNode *> elifConds;
    std::vector<Statements *> elifVec;
    Statements *elseVec;
};
#endif //APYTHONINTERPRETER_STATEMENTS_HPP
