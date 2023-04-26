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
    TypeDescriptor *rhs = rhsExpression()->evaluate(symTab);
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
    // std::cout << std::endl;
}


// PrintStatement
// ===========================================================================
// Print Constructors
//PrintStatement::PrintStatement() : _rhs{""} {}
//PrintStatement::PrintStatement(ExprNode *rhs) : _rhsID{rhs->token().getName()}{}

PrintStatement::PrintStatement() : _relExpressions{nullptr} {}
PrintStatement::PrintStatement(std::vector<ExprNode *> expressions) : _relExpressions{expressions} {}

//Member Functions get string
std::vector<ExprNode*> &PrintStatement::getExpressions(){
    return _relExpressions;
}

//Print the return value of whatever is being returned from the  symbol table map.
//In this case, we are getting the second element of the map which is an int.
void PrintStatement::evaluate(SymTab &symTab){
    TypeDescriptor *temp;
    for (auto s: _relExpressions){
        temp = s->evaluate(symTab);
        if(temp->getType() == TypeDescriptor::INTEGER)
            std::cout << dynamic_cast<IntegerTypeDescriptor *>(temp)->returnVal();
        else if(temp->getType() == TypeDescriptor::BOOL)
            std::cout << dynamic_cast<BoolTypeDescriptor *>(temp)->returnVal();
        else if(temp->getType() == TypeDescriptor::DOUBLE)
            std::cout << dynamic_cast<DoubleTypeDescriptor *>(temp)->returnVal();
        else if(temp->getType() == TypeDescriptor::STRING)
            std::cout << dynamic_cast<StringTypeDescriptor *>(temp)->returnVal();
        else{
            std::cout << "Error printing the variable's value\n";
            exit(1);
        }   
        if(_relExpressions.size() > 1)
            std::cout << " ";
    
    }
        
    
}

//Don't need to implement this
void PrintStatement::print(){
    std::cout << "print ";
    for(auto s: _relExpressions)
    {
        s->print();
    }
        
    std::cout << std::endl;
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
    std::cout << "\nPrinting for loop statement:\n";
    std::cout << "for(";
    left()->print();
    std::cout << "; ";
    mid()->print();
    std::cout << "; ";
    right()->print();

    std::cout << "){ \n";
    for(auto s: returnVec()){
        s->print();
        std::cout << std::endl;
    }
    std::cout << "}\n\n";
}

// Function to add our statements inside of the for-loop brackets to a vector inside
// the ForStatement subclass
void ForStatement::addToForLoopStatements(Statements *stmts) { _forLoopStatements.push_back(stmts); }

void ForStatement::evaluate(SymTab &symTab)
{
    int index = 0;
    unsigned int vecSize = returnVec().size();
    left()->evaluate(symTab);
    while(dynamic_cast<BoolTypeDescriptor *>(mid()->evaluate(symTab))->returnVal())
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


   

    



