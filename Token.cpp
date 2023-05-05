//
// Created by Ali Kooshesh on 4/4/23.
//

#include <iostream>
#include "Token.hpp"

Token::Token(): _name{""}, _eof{false}, _eol{false}, _symbol{'\0'}, _isWholeNumber{false}
                , _isKeyWord{false}, _isDouble{false}, _isString{false}, _isIndent{false}, _isOutdent{false} {}

void Token::print() const {
    if( eol() ) std::cout << "EOL\n" ;
    else if( eof() )                    std::cout << "EOF" ;
    else if( isOpenParen() )  std::cout << "(" ;
    else if( isDoubleNumber()) std::cout << getDoubleNumber();
    else if( isCloseParen() )  std::cout << ")" ;
    else if( isAssignmentOperator() )  std::cout << " = " ;
    else if( isClosedBrack() ) std::cout << " } ";
    else if(isOpenBrack() ) std::cout << " { ";
    else if(isComma() ) std::cout << " , ";
    else if(isColon() ) std::cout << " : ";
    else if( isSemiColon() )  std::cout << ";" ;
    else if(getIsIndent()) std::cout << "INDENT";
    else if(getIsOutdent()) std::cout << "OUTDENT";
    else if( isString() ) std::cout << getString();
    else if( isMultiplicationOperator() )  std::cout << " * " ;
    else if( isAdditionOperator() )  std::cout << " + " ;
    else if( isSubtractionOperator() )  std::cout << " - " ;
    else if( isModuloOperator() )  std::cout << " % " ;
    else if( isDivisionOperator() )  std::cout << " / " ;
    else if( isNotEqualSecondary() ) std::cout << " <> ";
    else if( isExtendedOpFloor() ) std::cout << " // ";
    else if( isName() )  std::cout <<  getName();
    else if( isWholeNumber() ) std::cout <<  getWholeNumber();
    else if ( isEqualOperator() ) std::cout << " == ";
    else if ( isNotEqualOperator() ) std::cout << " != ";
    else if ( isGreaterThanOperator() ) std::cout << " > ";
    else if ( isGreaterThanOrEqualToOperator() ) std::cout << " >= ";
    else if ( isLessThanOperator() ) std::cout << " < ";
    else if ( isLessThanOrEqualToOperator() ) std::cout << " <= ";
    else if ( isRelationalOperator() ) std::cout << getRelationalOperator();
    else std::cout << "Uninitialized token.\n";
}
