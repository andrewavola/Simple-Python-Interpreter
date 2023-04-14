//
// Created by Ali Kooshesh on 4/4/23.
//

#include "Statements.hpp"

// Statement
Statement::Statement() {}

// Statements

Statements::Statements() {}
void Statements::addStatement(Statement *statement) { _statements.push_back(statement); }

void Statements::print() {
    for (auto s: _statements)
        s->print();
}

// s is going to be the same type as whatever the type of subclass is held by _statements is
// so if we create another evaluate function in our subclasses, it should be 
// called properly.
void Statements::evaluate(SymTab &symTab) {
    for (auto s: _statements)
        s->evaluate(symTab);
}

// AssignmentStatement
// ===========================================================================
AssignmentStatement::AssignmentStatement() : _lhsVariable{""}, _rhsExpression{nullptr} {}

AssignmentStatement::AssignmentStatement(std::string lhsVar, ExprNode *rhsExpr):
        _lhsVariable{lhsVar}, _rhsExpression{rhsExpr} {}

void AssignmentStatement::evaluate(SymTab &symTab) {
    int rhs = rhsExpression()->evaluate(symTab);
    symTab.setValueFor(lhsVariable(), rhs);
}

std::string &AssignmentStatement::lhsVariable() {
    return _lhsVariable;
}

ExprNode *&AssignmentStatement::rhsExpression() {
    return _rhsExpression;
}

void AssignmentStatement::print() {
    std::cout << _lhsVariable << " = ";
    _rhsExpression->print();
    std::cout << std::endl;
}


// PrintStatement
// ===========================================================================
// Print Constructors
//PrintStatement::PrintStatement() : _rhs{""} {}
//PrintStatement::PrintStatement(ExprNode *rhs) : _rhsID{rhs->token().getName()}{}

PrintStatement::PrintStatement() : _rhsExpression{nullptr} {}
PrintStatement::PrintStatement(ExprNode *rhs) : _rhsExpression{rhs} {}

//Member Functions get string
ExprNode *&PrintStatement::rhsExpression(){
    return _rhsExpression;
}

//Print the return value of whatever is being returned from the  symbol table map.
//In this case, we are getting the second element of the map which is an int.
void PrintStatement::evaluate(SymTab &symTab){
    std::cout << symTab.getValueFor(rhsExpression()->token().getName()) << std::endl;
}

//Don't need to implement this
void PrintStatement::print(){
}

// ForLoopStatement
// ===========================================================================
// ForLoopStatement Constructors
ForStatement::ForStatement() : _losc{nullptr}, _middle{nullptr}, _rosc{nullptr} {}
ForStatement::ForStatement(AssignmentStatement *left, ExprNode *middle, AssignmentStatement *right, std::vector<Statements *> stmtVec)
{
    _losc = left;
    _middle = middle;
    _rosc = right;
    _forLoopStatements = stmtVec;
}

AssignmentStatement *&ForStatement::left()
{
    return _losc;
}

AssignmentStatement *&ForStatement::right()
{
    return _rosc;
}

ExprNode *&ForStatement::mid()
{
    return _middle;
}

std::vector<Statements*> ForStatement::returnVec(){
    return _forLoopStatements;
}

void ForStatement::print()
{

}

// Function to add our statements inside of the for-loop brackets to a vector inside
// the ForStatement subclass
void ForStatement::addToForLoopStatements(Statements *stmts) { _forLoopStatements.push_back(stmts); }

void ForStatement::evaluate(SymTab &symTab)
{
    int index = 0;
    unsigned int vecSize = returnVec().size();
    left()->evaluate(symTab);
    while(mid()->evaluate(symTab) == 1)
    {
        while(index < vecSize)
        {
            returnVec().at(index)->evaluate(symTab);
            index++;
            
        }
        index = 0;
        right()->evaluate(symTab);
    }
}


   

    



