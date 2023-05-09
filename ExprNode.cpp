//
// Created by Ali Kooshesh on 4/4/23.
//


#include<iostream>
#include "ExprNode.hpp"
#include <cmath>
#include <string.h>
// ExprNode
ExprNode::ExprNode(Token token): _token{token} {}

Token ExprNode::token() { return _token; }

// InfixExprNode functions
InfixExprNode::InfixExprNode(Token tk) : ExprNode{tk}, _left(nullptr), _right(nullptr) {}

ExprNode *&InfixExprNode::left() { return _left; }

ExprNode *&InfixExprNode::right() { return _right; }

TypeDescriptor* InfixExprNode::evaluate(SymTab &symTab) {

    
    bool bothInt = false; // int
    bool intAndDouble = false; // double
    bool bothDouble = false; // double
    bool bothStrings = false;
    bool boolVals = false;
    bool lhsDouble = false;
    bool rhsDouble = false;
    bool isNotOp = false;
    TypeDescriptor *lValue;
    TypeDescriptor *rValue;
    // Evaluates an infix expression using a post-order traversal of the expression tree.
    if(!(left() == nullptr)){
        lValue = left()->evaluate(symTab);
    }
        
    if(!(right() == nullptr))
        rValue = right()->evaluate(symTab);
        
    
    if(debug)
        std::cout << "InfixExprNode::evaluate: " << lValue << " " <<
            token().symbol() << " " << rValue << std::endl;

    if(token().isNotOp())
        isNotOp = true;
    //Handles what values we have in our downcasted TypeDescriptors
    else if(lValue->getType() == TypeDescriptor::INTEGER && rValue->getType() == TypeDescriptor::INTEGER)
    {
        bothInt = true;
    }
        
    else if(lValue->getType() == TypeDescriptor::INTEGER && rValue->getType() == TypeDescriptor::DOUBLE)
    {
        intAndDouble = true;
        rhsDouble = true;
    }
    else if(lValue->getType() == TypeDescriptor::DOUBLE && rValue->getType() == TypeDescriptor::INTEGER)
    {
        intAndDouble = true;
        lhsDouble = true;
    }
    else if(lValue->getType() == TypeDescriptor::DOUBLE && rValue->getType() == TypeDescriptor::DOUBLE)
        bothDouble = true;
    else if(lValue->getType() == TypeDescriptor::STRING && rValue->getType() == TypeDescriptor::STRING)
        bothStrings = true;
    else if(lValue->getType() == TypeDescriptor::BOOL && rValue->getType() == TypeDescriptor::BOOL)
        boolVals = true;
    
    else
    {
        std::cout << "Incompatible types for operations\n";
        exit(1);
    }
        
    //Operations

    if(token().isNotOp()){
        //IF we have no expression to the right of NOT
        if(lValue->getType() == TypeDescriptor::INTEGER){
            int returnVal = dynamic_cast<IntegerTypeDescriptor *>(lValue)->returnVal();
            return new BoolTypeDescriptor(!returnVal, TypeDescriptor::BOOL);
        }
        else if(lValue->getType() == TypeDescriptor::BOOL){
            bool returnVal = dynamic_cast<BoolTypeDescriptor *>(lValue)->returnVal();
            return new BoolTypeDescriptor(!returnVal, TypeDescriptor::BOOL);
        }
          
    }
    //Addition
    if(token().isAdditionOperator())
    {
        if(bothInt)
        {
            
            int returnVal = dynamic_cast<IntegerTypeDescriptor *>(lValue)->returnVal() 
                + dynamic_cast<IntegerTypeDescriptor *>(rValue)->returnVal();
            return new IntegerTypeDescriptor(returnVal, TypeDescriptor::INTEGER);
        }
        else if(bothDouble)
        {
            double returnVal = dynamic_cast<DoubleTypeDescriptor *>(lValue)->returnVal()
                + dynamic_cast<DoubleTypeDescriptor *>(rValue)->returnVal();
            return new DoubleTypeDescriptor(returnVal, TypeDescriptor::DOUBLE);
        }
        else if(intAndDouble)
        {
            if(lhsDouble)
            {
                double returnVal = dynamic_cast<DoubleTypeDescriptor *>(lValue)->returnVal()
                    + dynamic_cast<IntegerTypeDescriptor *>(rValue)->returnVal();
            return new DoubleTypeDescriptor(returnVal, TypeDescriptor::DOUBLE);
            }
            else{
                double returnVal = dynamic_cast<IntegerTypeDescriptor *>(lValue)->returnVal()
                    + dynamic_cast<DoubleTypeDescriptor *>(rValue)->returnVal();
            return new DoubleTypeDescriptor(returnVal, TypeDescriptor::DOUBLE);
            }
        }
        else if(bothStrings)
        {
            std::string concatString = dynamic_cast<StringTypeDescriptor *>(lValue)->returnVal() + dynamic_cast<StringTypeDescriptor *>(rValue)->returnVal();
            return new StringTypeDescriptor(concatString, TypeDescriptor::STRING);
        }
        else{
            std::cout << "Error performing addition operator with inputed values\n";
            exit(1);
        }
                 
    }
    //Subtraction
    else if(token().isSubtractionOperator())
    {
        if(bothInt)
        {
            int returnVal = dynamic_cast<IntegerTypeDescriptor *>(lValue)->returnVal() 
                - dynamic_cast<IntegerTypeDescriptor *>(rValue)->returnVal();
            return new IntegerTypeDescriptor(returnVal, TypeDescriptor::INTEGER);
        }
        else if(bothDouble)
        {
            double returnVal = dynamic_cast<DoubleTypeDescriptor *>(lValue)->returnVal()
                - dynamic_cast<DoubleTypeDescriptor *>(rValue)->returnVal();
            return new DoubleTypeDescriptor(returnVal, TypeDescriptor::DOUBLE);
        }
        else if(intAndDouble)
        {
            if(lhsDouble)
            {
                double returnVal = dynamic_cast<DoubleTypeDescriptor *>(lValue)->returnVal()
                    - dynamic_cast<IntegerTypeDescriptor *>(rValue)->returnVal();
            return new DoubleTypeDescriptor(returnVal, TypeDescriptor::DOUBLE);
            }
            else{
                double returnVal = dynamic_cast<IntegerTypeDescriptor *>(lValue)->returnVal()
                    - dynamic_cast<DoubleTypeDescriptor *>(rValue)->returnVal();
            return new DoubleTypeDescriptor(returnVal, TypeDescriptor::DOUBLE);
            }
        }
        else{
            std::cout << "Error performing Addition operator with inputed values\n";
            exit(1);
        }

    }
    //Multiplication
    else if(token().isMultiplicationOperator())
    {
        if(bothInt)
        {
            int returnVal = dynamic_cast<IntegerTypeDescriptor *>(lValue)->returnVal() 
                * dynamic_cast<IntegerTypeDescriptor *>(rValue)->returnVal();
            return new IntegerTypeDescriptor(returnVal, TypeDescriptor::INTEGER);
        }
        else if(bothDouble)
        {
            double returnVal = dynamic_cast<DoubleTypeDescriptor *>(lValue)->returnVal()
                * dynamic_cast<DoubleTypeDescriptor *>(rValue)->returnVal();
            return new DoubleTypeDescriptor(returnVal, TypeDescriptor::DOUBLE);
        }
        else if(intAndDouble)
        {
            if(lhsDouble)
            {
                double returnVal = dynamic_cast<DoubleTypeDescriptor *>(lValue)->returnVal()
                    * dynamic_cast<IntegerTypeDescriptor *>(rValue)->returnVal();
            return new DoubleTypeDescriptor(returnVal, TypeDescriptor::DOUBLE);
            }
            else{
                double returnVal = dynamic_cast<IntegerTypeDescriptor *>(lValue)->returnVal()
                    * dynamic_cast<DoubleTypeDescriptor *>(rValue)->returnVal();
            return new DoubleTypeDescriptor(returnVal, TypeDescriptor::DOUBLE);
            }
        }
        else{
            std::cout << "Error performing Multiplication operator with inputed values\n";
            exit(1);
        }
    }
    //Division
    else if(token().isDivisionOperator())
    {
        if(bothInt)
        {
            int returnVal = dynamic_cast<IntegerTypeDescriptor *>(lValue)->returnVal() 
                / dynamic_cast<IntegerTypeDescriptor *>(rValue)->returnVal();
            return new IntegerTypeDescriptor(returnVal, TypeDescriptor::INTEGER);
        }
        else if(bothDouble)
        {
            double returnVal = dynamic_cast<DoubleTypeDescriptor *>(lValue)->returnVal()
                / dynamic_cast<DoubleTypeDescriptor *>(rValue)->returnVal();
            return new DoubleTypeDescriptor(returnVal, TypeDescriptor::DOUBLE);
        }
        else if(intAndDouble)
        {
            if(lhsDouble)
            {
                double returnVal = dynamic_cast<DoubleTypeDescriptor *>(lValue)->returnVal()
                    / dynamic_cast<IntegerTypeDescriptor *>(rValue)->returnVal();
            return new DoubleTypeDescriptor(returnVal, TypeDescriptor::DOUBLE);
            }
            else{
                double returnVal = dynamic_cast<IntegerTypeDescriptor *>(lValue)->returnVal()
                    / dynamic_cast<DoubleTypeDescriptor *>(rValue)->returnVal();
            return new DoubleTypeDescriptor(returnVal, TypeDescriptor::DOUBLE);
            }
        }
        else{
            std::cout << "Error performing Division operator with inputed values\n";
            exit(1);
        }
    }
    //Only works with INTS
    else if(token().isModuloOperator())
    {
        if(bothInt)
        {
            int returnVal = dynamic_cast<IntegerTypeDescriptor *>(lValue)->returnVal() 
                % dynamic_cast<IntegerTypeDescriptor *>(rValue)->returnVal();
            return new IntegerTypeDescriptor(returnVal, TypeDescriptor::INTEGER);
        }
        else{
            std::cout << "Error performing Modulo operator with inputed values\n";
            exit(1);
        }
    }

    //Is Equal
    else if(token().isEqualOperator())
    {
        if(bothInt)
        {
            bool returnVal = (dynamic_cast<IntegerTypeDescriptor *>(lValue)->returnVal() 
                == dynamic_cast<IntegerTypeDescriptor *>(rValue)->returnVal());
            return new BoolTypeDescriptor(returnVal, TypeDescriptor::BOOL);
        }
        else if(bothDouble)
        {
            bool returnVal = (dynamic_cast<DoubleTypeDescriptor *>(lValue)->returnVal()
                == dynamic_cast<DoubleTypeDescriptor *>(rValue)->returnVal());
            return new BoolTypeDescriptor(returnVal, TypeDescriptor::BOOL);
        }
        else if(intAndDouble)
        {
            if(lhsDouble)
            {
                bool returnVal = dynamic_cast<DoubleTypeDescriptor *>(lValue)->returnVal()
                    == dynamic_cast<IntegerTypeDescriptor *>(rValue)->returnVal();
            return new BoolTypeDescriptor(returnVal, TypeDescriptor::BOOL);
            }
            else{
                bool returnVal = dynamic_cast<IntegerTypeDescriptor *>(lValue)->returnVal()
                    == dynamic_cast<DoubleTypeDescriptor *>(rValue)->returnVal();
            return new BoolTypeDescriptor(returnVal, TypeDescriptor::BOOL);
            }
        }
        else if(bothStrings)
        {
            int retVal = strcmp(dynamic_cast<StringTypeDescriptor *>(lValue)->returnVal().c_str(), dynamic_cast<StringTypeDescriptor *>(rValue)->returnVal().c_str());
            if(retVal == 0)
                return new BoolTypeDescriptor(true, TypeDescriptor::BOOL);
            else
                return new BoolTypeDescriptor(false, TypeDescriptor::BOOL);
        }
        else{
            std::cout << "Error performing IsEqual operator with inputed values\n";
            exit(1);
        }
    }
    else if(token().isNotEqualOperator() || token().isNotEqualSecondary())
    {
        if(bothInt)
        {
            bool returnVal = (dynamic_cast<IntegerTypeDescriptor *>(lValue)->returnVal() 
                != dynamic_cast<IntegerTypeDescriptor *>(rValue)->returnVal());
            return new BoolTypeDescriptor(returnVal, TypeDescriptor::BOOL);
        }
        else if(bothDouble)
        {
            bool returnVal = (dynamic_cast<DoubleTypeDescriptor *>(lValue)->returnVal()
                != dynamic_cast<DoubleTypeDescriptor *>(rValue)->returnVal());
            return new BoolTypeDescriptor(returnVal, TypeDescriptor::BOOL);
        }
        else if(intAndDouble)
        {
            if(lhsDouble)
            {
                bool returnVal = dynamic_cast<DoubleTypeDescriptor *>(lValue)->returnVal()
                    != dynamic_cast<IntegerTypeDescriptor *>(rValue)->returnVal();
            return new BoolTypeDescriptor(returnVal, TypeDescriptor::BOOL);
            }
            else{
                bool returnVal = dynamic_cast<IntegerTypeDescriptor *>(lValue)->returnVal()
                    != dynamic_cast<DoubleTypeDescriptor *>(rValue)->returnVal();
            return new BoolTypeDescriptor(returnVal, TypeDescriptor::BOOL);
            }
        }
        else if(bothStrings)
        {
            int retVal = strcmp(dynamic_cast<StringTypeDescriptor *>(lValue)->returnVal().c_str(), dynamic_cast<StringTypeDescriptor *>(rValue)->returnVal().c_str());
            if(retVal == 0)
                return new BoolTypeDescriptor(false, TypeDescriptor::BOOL);
            else
                return new BoolTypeDescriptor(true, TypeDescriptor::BOOL);
        }
        else{
            std::cout << "Error performing IsNotEqual operator with inputed values\n";
            exit(1);
        }
    }

    else if(token().isGreaterThanOperator())
    {
        if(bothInt)
        {
            bool returnVal = (dynamic_cast<IntegerTypeDescriptor *>(lValue)->returnVal() 
                > dynamic_cast<IntegerTypeDescriptor *>(rValue)->returnVal());
            return new BoolTypeDescriptor(returnVal, TypeDescriptor::BOOL);
        }
        else if(bothDouble)
        {
            bool returnVal = (dynamic_cast<DoubleTypeDescriptor *>(lValue)->returnVal()
                > dynamic_cast<DoubleTypeDescriptor *>(rValue)->returnVal());
            return new BoolTypeDescriptor(returnVal, TypeDescriptor::BOOL);
        }
        else if(intAndDouble)
        {
            if(lhsDouble)
            {
                bool returnVal = dynamic_cast<DoubleTypeDescriptor *>(lValue)->returnVal()
                    > dynamic_cast<IntegerTypeDescriptor *>(rValue)->returnVal();
            return new BoolTypeDescriptor(returnVal, TypeDescriptor::BOOL);
            }
            else{
                bool returnVal = dynamic_cast<IntegerTypeDescriptor *>(lValue)->returnVal()
                    > dynamic_cast<DoubleTypeDescriptor *>(rValue)->returnVal();
            return new BoolTypeDescriptor(returnVal, TypeDescriptor::BOOL);
            }
        }
          else{
            std::cout << "Error performing GreaterThan operator with inputed values\n";
            exit(1);
        }
    }
    else if(token().isLessThanOperator())
    {
        if(bothInt)
        {
            bool returnVal = (dynamic_cast<IntegerTypeDescriptor *>(lValue)->returnVal() 
                < dynamic_cast<IntegerTypeDescriptor *>(rValue)->returnVal());
            return new BoolTypeDescriptor(returnVal, TypeDescriptor::BOOL);
        }
        else if(bothDouble)
        {
            bool returnVal = (dynamic_cast<DoubleTypeDescriptor *>(lValue)->returnVal()
                < dynamic_cast<DoubleTypeDescriptor *>(rValue)->returnVal());
            return new BoolTypeDescriptor(returnVal, TypeDescriptor::BOOL);
        }
        else if(intAndDouble)
        {
            if(lhsDouble)
            {
                bool returnVal = dynamic_cast<DoubleTypeDescriptor *>(lValue)->returnVal()
                    < dynamic_cast<IntegerTypeDescriptor *>(rValue)->returnVal();
            return new BoolTypeDescriptor(returnVal, TypeDescriptor::BOOL);
            }
            else{
                bool returnVal = dynamic_cast<IntegerTypeDescriptor *>(lValue)->returnVal()
                    < dynamic_cast<DoubleTypeDescriptor *>(rValue)->returnVal();
            return new BoolTypeDescriptor(returnVal, TypeDescriptor::BOOL);
            }
        }
          else{
            std::cout << "Error performing LessThan operator with inputed values\n";
            exit(1);
        }
    }
    else if(token().isGreaterThanOrEqualToOperator())
    {
        if(bothInt)
        {
            bool returnVal = (dynamic_cast<IntegerTypeDescriptor *>(lValue)->returnVal() 
                >= dynamic_cast<IntegerTypeDescriptor *>(rValue)->returnVal());
            return new BoolTypeDescriptor(returnVal, TypeDescriptor::BOOL);
        }
        else if(bothDouble)
        {
            bool returnVal = (dynamic_cast<DoubleTypeDescriptor *>(lValue)->returnVal()
                >= dynamic_cast<DoubleTypeDescriptor *>(rValue)->returnVal());
            return new BoolTypeDescriptor(returnVal, TypeDescriptor::BOOL);
        }
        else if(intAndDouble)
        {
            if(lhsDouble)
            {
                bool returnVal = dynamic_cast<DoubleTypeDescriptor *>(lValue)->returnVal()
                    >= dynamic_cast<IntegerTypeDescriptor *>(rValue)->returnVal();
            return new BoolTypeDescriptor(returnVal, TypeDescriptor::BOOL);
            }
            else{
                bool returnVal = dynamic_cast<IntegerTypeDescriptor *>(lValue)->returnVal()
                    >= dynamic_cast<DoubleTypeDescriptor *>(rValue)->returnVal();
            return new BoolTypeDescriptor(returnVal, TypeDescriptor::BOOL);
            }
        }
          else{
            std::cout << "Error performing GreaterThanOrEqualTo operator with inputed values\n";
            exit(1);
        }
    }
    else if(token().isLessThanOrEqualToOperator())
    {
        if(bothInt)
        {
            bool returnVal = (dynamic_cast<IntegerTypeDescriptor *>(lValue)->returnVal() 
                <= dynamic_cast<IntegerTypeDescriptor *>(rValue)->returnVal());
            return new BoolTypeDescriptor(returnVal, TypeDescriptor::BOOL);
        }
        else if(bothDouble)
        {
            bool returnVal = (dynamic_cast<DoubleTypeDescriptor *>(lValue)->returnVal()
                <= dynamic_cast<DoubleTypeDescriptor *>(rValue)->returnVal());
            return new BoolTypeDescriptor(returnVal, TypeDescriptor::BOOL);
        }
        else if(intAndDouble)
        {
            if(lhsDouble)
            {
                bool returnVal = dynamic_cast<DoubleTypeDescriptor *>(lValue)->returnVal()
                    <= dynamic_cast<IntegerTypeDescriptor *>(rValue)->returnVal();
            return new BoolTypeDescriptor(returnVal, TypeDescriptor::BOOL);
            }
            else{
                bool returnVal = dynamic_cast<IntegerTypeDescriptor *>(lValue)->returnVal()
                    <= dynamic_cast<DoubleTypeDescriptor *>(rValue)->returnVal();
            return new BoolTypeDescriptor(returnVal, TypeDescriptor::BOOL);
            }
        }
          else{
            std::cout << "Error performing LessThanEqualTo operator with inputed values\n";
            exit(1);
        }
    }
    //floor division
    else if(token().isExtendedOpFloor())
    {
        if(bothInt)
        {
            int returnVal = floor((dynamic_cast<IntegerTypeDescriptor *>(lValue)->returnVal() 
                / dynamic_cast<IntegerTypeDescriptor *>(rValue)->returnVal()));
            return new IntegerTypeDescriptor(returnVal, TypeDescriptor::INTEGER);
        }
        else if(bothDouble)
        {
            int returnVal = floor((dynamic_cast<DoubleTypeDescriptor *>(lValue)->returnVal()
                / dynamic_cast<DoubleTypeDescriptor *>(rValue)->returnVal()));
            return new IntegerTypeDescriptor(returnVal, TypeDescriptor::INTEGER);
        }
        else if(intAndDouble)
        {
            if(lhsDouble)
            {
                int returnVal = floor(dynamic_cast<DoubleTypeDescriptor *>(lValue)->returnVal()
                    / dynamic_cast<IntegerTypeDescriptor *>(rValue)->returnVal());
            return new IntegerTypeDescriptor(returnVal, TypeDescriptor::INTEGER);
            }
            else{
                int returnVal = floor(dynamic_cast<IntegerTypeDescriptor *>(lValue)->returnVal()
                    / dynamic_cast<DoubleTypeDescriptor *>(rValue)->returnVal());
            return new IntegerTypeDescriptor(returnVal, TypeDescriptor::INTEGER);
            }
        }
          else{
            std::cout << "Error performing Floor operator with inputed values\n";
            exit(1);
        }
    }
    else if(token().isAndOp()){
        if(bothInt)
        {
            bool returnVal = (dynamic_cast<IntegerTypeDescriptor *>(lValue)->returnVal() 
                && dynamic_cast<IntegerTypeDescriptor *>(rValue)->returnVal());
            return new BoolTypeDescriptor(returnVal, TypeDescriptor::BOOL);
        }
        else if(bothDouble)
        {
            bool returnVal = (dynamic_cast<DoubleTypeDescriptor *>(lValue)->returnVal()
                && dynamic_cast<DoubleTypeDescriptor *>(rValue)->returnVal());
            return new BoolTypeDescriptor(returnVal, TypeDescriptor::BOOL);
        }
        else if(intAndDouble)
        {
            if(lhsDouble)
            {
                bool returnVal = dynamic_cast<DoubleTypeDescriptor *>(lValue)->returnVal()
                    && dynamic_cast<IntegerTypeDescriptor *>(rValue)->returnVal();
            return new BoolTypeDescriptor(returnVal, TypeDescriptor::BOOL);
            }
            else{
                bool returnVal = dynamic_cast<IntegerTypeDescriptor *>(lValue)->returnVal()
                    && dynamic_cast<DoubleTypeDescriptor *>(rValue)->returnVal();
            return new BoolTypeDescriptor(returnVal, TypeDescriptor::BOOL);
            }
        }
        else if(boolVals){
            bool returnVal = dynamic_cast<BoolTypeDescriptor *>(lValue)->returnVal()
                && dynamic_cast<BoolTypeDescriptor *>(rValue)->returnVal();
            return new BoolTypeDescriptor(returnVal, TypeDescriptor::BOOL);
        }
          else{
            // std::cout <<  dynamic_cast<IntegerTypeDescriptor*>(lValue)->getType();
            // std::cout << ", ";
            // std::cout << dynamic_cast<IntegerTypeDescriptor*>(rValue)->getType();
            std::cout << "Error performing AND operator with inputed values\n";
            exit(1);
        }
    }
    
    //Error check
    else {
        std::cout << "InfixExprNode::evaluate: don't know how to evaluate this operator\n";
        token().print();
        std::cout << std::endl;
        exit(2);
    }


    
}

void InfixExprNode::print() {
    if(!(_left == nullptr))
        _left->print();
    token().print();
    if(!(_right == nullptr))
        _right->print();
}

// WHoleNumber
WholeNumber::WholeNumber(Token token): ExprNode{token} {}

void WholeNumber::print() {
    token().print();
}

IntegerTypeDescriptor* WholeNumber::evaluate(SymTab &symTab) {
    if(debug)
        std::cout << "WholeNumber::evaluate: returning " << token().getWholeNumber() << std::endl;
    
    return new IntegerTypeDescriptor(token().getWholeNumber(), TypeDescriptor::INTEGER);
    // return dynamic_cast<IntegerTypeDescriptor*>(symTab.getValueFor(token().getName()));
    //return token().getWholeNumber();
}

// Variable

Variable::Variable(Token token): ExprNode{token} {}

void Variable::print() {
    token().print();
}

TypeDescriptor* Variable::evaluate(SymTab &symTab) {
    if( ! symTab.isDefined(token().getName())) {
        std::cout << "Use of undefined variable, " << token().getName() << std::endl;
        exit(1);
    }
    if(debug)
        std::cout << "Variable::evaluate: returning " << symTab.getValueFor(token().getName()) << std::endl;

     
    return symTab.getValueFor(token().getName());
}

//Double
DoubleNumber::DoubleNumber(Token token) : ExprNode(token) {}

void DoubleNumber::print() {
    token().print();
}

DoubleTypeDescriptor* DoubleNumber::evaluate(SymTab &symTab){
    if(debug)
        std::cout << "DoubleNumber::evaluate: returning " << symTab.getValueFor(token().getName()) << std::endl;
    return new DoubleTypeDescriptor(token().getDoubleNumber(), TypeDescriptor::DOUBLE);
}

//String
StringValue::StringValue(Token token) : ExprNode(token) {}

void StringValue::print(){
    token().print();
}

StringTypeDescriptor* StringValue::evaluate(SymTab &symTab){
    if(debug)
        std::cout << "StringValue::evaluate: returning " << symTab.getValueFor(token().getName()) << std::endl;
    
    return new StringTypeDescriptor(token().getString(), TypeDescriptor::STRING);
}
