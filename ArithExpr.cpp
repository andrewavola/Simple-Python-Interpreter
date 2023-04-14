//
// Created by Ali Kooshesh on 4/4/23.
//


#include<iostream>
#include "ArithExpr.hpp"

// ArithExprNode
ArithExprNode::ArithExprNode(Token token): _token{token} {}

Token ArithExprNode::token() { return _token; }

// InfixExprNode functions
InfixExprNode::InfixExprNode(Token tk) : ArithExprNode{tk}, _left(nullptr), _right(nullptr) {}

ArithExprNode *&InfixExprNode::left() { return _left; }

ArithExprNode *&InfixExprNode::right() { return _right; }

int InfixExprNode::evaluate(SymTab &symTab) {
    // Evaluates an infix expression using a post-order traversal of the expression tree.
    int lValue = left()->evaluate(symTab);
    int rValue = right()->evaluate(symTab);
    if(debug)
        std::cout << "InfixExprNode::evaluate: " << lValue << " " <<
            token().symbol() << " " << rValue << std::endl;
    if( token().isAdditionOperator() )
        return lValue + rValue;
    else if(token().isSubtractionOperator())
        return lValue - rValue;
    else if(token().isMultiplicationOperator())
        return lValue * rValue;
    else if(token().isDivisionOperator())
        return lValue / rValue; // division by zero?
    else if( token().isModuloOperator() )
        return lValue % rValue;
    else {
        std::cout << "InfixExprNode::evaluate: don't know how to evaluate this operator\n";
        token().print();
        std::cout << std::endl;
        exit(2);
    }
}

void InfixExprNode::print() {
    _left->print();
    token().print();
    _right->print();
}

// WHoleNumber
WholeNumber::WholeNumber(Token token): ArithExprNode{token} {}

void WholeNumber::print() {
    token().print();
}

int WholeNumber::evaluate(SymTab &symTab) {
    if(debug)
        std::cout << "WholeNumber::evaluate: returning " << token().getWholeNumber() << std::endl;
    return token().getWholeNumber();
}

// Variable

Variable::Variable(Token token): ArithExprNode{token} {}

void Variable::print() {
    token().print();
}

int Variable::evaluate(SymTab &symTab) {
    if( ! symTab.isDefined(token().getName())) {
        std::cout << "Use of undefined variable, " << token().getName() << std::endl;
        exit(1);
    }
    if(debug)
        std::cout << "Variable::evaluate: returning " << symTab.getValueFor(token().getName()) << std::endl;
    return symTab.getValueFor(token().getName());
}

