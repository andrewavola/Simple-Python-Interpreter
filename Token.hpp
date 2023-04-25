//
// Created by Ali Kooshesh on 4/4/23.
//

#ifndef APYTHONINTERPRETER_TOKEN_HPP
#define APYTHONINTERPRETER_TOKEN_HPP

#include<string>
class Token {

public:
    Token();

    bool &eof()  { return _eof; }
    bool &eol()  { return _eol; }

    bool eof() const { return _eof; }
    bool eol() const  { return _eol; }

    bool isOpenParen() const  { return _symbol == '('; }
    bool isCloseParen() const { return _symbol == ')'; }

    //for For loop statement
    bool isOpenBrack() const { return _symbol == '{';}
    bool isClosedBrack() const { return _symbol == '}';}

    void symbol(char c) { _symbol = c; }
    char symbol() { return _symbol; }

    bool isComma() const {return _symbol == ',';}
    bool isSemiColon() const { return _symbol == ';'; }
    bool isAssignmentOperator() const              { return _symbol == '='; }
    bool isMultiplicationOperator() const { return _symbol == '*'; }
    bool isAdditionOperator() const       { return _symbol == '+'; }
    bool isSubtractionOperator() const    { return _symbol == '-'; }
    bool isModuloOperator() const         { return _symbol == '%'; }
    bool isDivisionOperator() const       { return _symbol == '/'; }   
    bool isArithmeticOperator() const {
        return isMultiplicationOperator() ||
               isAdditionOperator() ||
               isSubtractionOperator() ||
               isModuloOperator() ||
               isDivisionOperator();
    }

    //Detect new line characters to terminate assign and print statements
    //bool isNewLineChar() const {return _symbol == '\n';}

    bool isName() const                   { return _name.length() > 0; }
    std::string getName() const                  { return _name; }
    void setName(std::string n) { _name = n; }

    bool &isWholeNumber() { return _isWholeNumber; }
    bool isWholeNumber() const { return _isWholeNumber; }
    int getWholeNumber() const { return _wholeNumber; }

    //Double logic inline functions
    bool &isDoubleNumber() {return _isDouble;}
    bool isDoubleNumber() const { return _isDouble;}
    double getDoubleNumber() const { return _double;}
    void setDoubleNumber(double n){
        _double = n;
        isDoubleNumber() = true;
    }

    void setWholeNumber(int n) {
        _wholeNumber = n;
        isWholeNumber() = true;
    }

    //String logic inline functions
    bool &isString(){ return _isString;}
    bool isString() const {return _isString;}
    std::string getString() const {return _string;}
    void setStringValue(std::string n){
        _string = n;
        isString() = true;
    }

    void print() const;

    bool isEqualOperator() const { return _relationalOperator == "==";}
    bool isNotEqualOperator() const { return _relationalOperator == "!=";}
    bool isGreaterThanOperator() const { return _relationalOperator == ">";}
    bool isGreaterThanOrEqualToOperator() const { return _relationalOperator == ">=";}
    bool isLessThanOperator() const { return _relationalOperator == "<";}
    bool isLessThanOrEqualToOperator() const { return _relationalOperator == "<=";}
    bool isNotEqualSecondary() const {return _relationalOperator == "<>";}
    bool isExtendedOpFloor() const {return _extendedOp == "//";}
    bool &isRelationalOperator() { return _isRelationOperator; }
    bool isRelationalOperator() const { return _isRelationOperator; }
    std::string getRelationalOperator() const {return _relationalOperator; }
    void setRelationalOperator(std::string str) {
        _relationalOperator = str;
        isRelationalOperator() = true;
    }

    void setExtendedOp(std::string str){
        _extendedOp = str;
    }
    
    //Step 5:
    bool isKeyword() {return (getName() == "for") ||  (getName() == "print");}
    void setIsKeyword(){_isKeyWord = true;}

private:
    std::string _name;
    bool _eof, _eol;
    bool _isWholeNumber;
    char _symbol;
    int _wholeNumber;
    bool _isKeyWord;
    bool _isDouble;
    double _double;
    bool _isRelationOperator;
    std::string _string;
    bool _isString;
    std::string _relationalOperator;
    std::string _extendedOp;
};


#endif //APYTHONINTERPRETER_TOKEN_HPP
