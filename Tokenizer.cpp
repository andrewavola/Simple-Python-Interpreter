//
// Created by Ali Kooshesh on 4/4/23.
//

#include <iostream>
#include <string>
#include <algorithm>
#include "Tokenizer.hpp"

std::string Tokenizer::readName() {
    // This function is called when it is known that
    // the first character in input is an alphabetic character.
    // The function reads and returns all characters of the name.

    std::string name;
    char c;

    // isalnum checks if 'c' is a decimal dig, upper/lower case letter
    // instream.get(c) takes one character and will either return true/false in
    // the case of our while loop

    while( inStream.get(c) && isalnum(c) ) {
        name += c;
    }

    // WHITESPACE CHECKER
    if(inStream.good())  // In the loop, we have read one char too many.
        inStream.putback(c);
    
    return name;
}

std::string Tokenizer::readString(){
    char c;
    std::string temp = "";
    while(inStream.get(c) && c != '\n' && c != '"'){
        temp += c;
    }
    if(c == '\n' || c != '"'){
        std::cout << "Ended with newline char or did not terminate with end quote\n";
        exit(1);
    }
    return temp;
}

int Tokenizer::readInteger() {
    // This function is called when it is known that
    // the first character in input is a digit.
    // The function reads and returns all remaining digits.

    int intValue = 0;
    char c;
    while( inStream.get(c) && isdigit(c) ) {
        //This seems to be a conversion from character to int 
        if(!isNegative)
            intValue = intValue * 10 + c - '0';
        else
            intValue = (0 - (intValue * 10 + c - '0'));
    }

    // WHITESPACE CHECKER
    if(inStream.good())  // In the loop, we have read one char too many.
        inStream.putback(c);
    return intValue;
}

double Tokenizer::readDouble(int front, int behind) {
    std::string tempString = "";
    std::string frontPeriod = "";
    std::string behindPeriod = "";
    frontPeriod = std::to_string(front);
    behindPeriod = std::to_string(behind);
    tempString += frontPeriod;
    tempString += ".";
    tempString += behindPeriod;
    return std::stod(tempString);
}
std::string Tokenizer::readRelationalOperator() {
    // This function is called when it is known that
    // the first character is a relational operator that is not ==
    // The function reads and returns >, >=, <, or <=

    std::string relationOp = "";
    char c;
   
    inStream.get(c);

    if (c == '<')
    {
        relationOp += c;
        inStream.get(c);
        if((c == '=' or c == '>') && relationOp.size() == 1)
            relationOp += c;
    }
    else
    {
        relationOp += c;
        inStream.get(c);
        if(c == '=' && relationOp.size() == 1)
            relationOp += c;
    }
       
    inStream.get(c);
    if(inStream.good())  // In the loop, we have read one char too many.
        inStream.putback(c);
    return relationOp;
}

//CONSTRUCTOR 
// - sets a reference to the passed inputstream reference to inStream in private section of class
// - defaults ungottenToken to false
// - lastToken is set to null
Tokenizer::Tokenizer(std::ifstream &stream): ungottenToken{false}, inStream{stream}, lastToken{}{
    indentStack.push_back(0);
    setParsingNewLine(true);
}


Token Tokenizer::getToken() {

    if(ungottenToken) {
        ungottenToken = false;
        return lastToken;
    }
    int spaceCounter = 0;
    char c;
    
    //Part of step 1 of phase 2
    while( inStream.get(c) && isspace(c) && c != '\n' ){
        spaceCounter++;
         
    } // Skip spaces but not new-line chars.

    
    if(getParsingNewLine())
    {
        if(c == '\n')
            spaceCounter = 0;
        else if(spaceCounter > getIndentStack().back()){
            Token indentTok;
            indentTok.setIsIndent();
            getIndentStack().push_back(spaceCounter);
            setParsingNewLine(false);
            inStream.putback(c);
            _tokens.push_back(indentTok);
            return lastToken = indentTok;
        }
        else if(spaceCounter < getIndentStack().back())
        {
            if(std::find(getIndentStack().begin(), getIndentStack().end(), spaceCounter) == getIndentStack().end())
            {
                std::cout << "Program has unaligned indentation level\n";
                exit(1);
            }
            setParsingNewLine(false);
            Token outdentTok;
            outdentTok.setIsOutdent();
            getIndentStack().pop_back();
            inStream.putback(c);
            _tokens.push_back(outdentTok);
            return lastToken = outdentTok;
            
        }
        
    }

    

    

    if(inStream.bad()) {
        std::cout << "Error while reading the input stream in Tokenizer.\n";
        exit(1);
    }

    //std::cout << "c = " << c << std::endl;

    Token token;
    
    setParsingNewLine(false);
    if( inStream.eof()) {
        token.eof() = true;
    } else if( c == '\n' ) {  // will not ever be the case unless new-line characters are not supressed.
        token.eol() = true;
        setParsingNewLine(true);
    } else if( isdigit(c)) { // a integer?
        // put the digit back into the input stream so
        // we read the entire number in a function
        inStream.putback(c);
        int behindPeriod;
        int frontPeriod = readInteger();
        inStream.get(c);
        if(c == '.'){
            behindPeriod = readInteger();
            token.setDoubleNumber(readDouble(frontPeriod, behindPeriod));
        }
        else{
            token.setWholeNumber(frontPeriod);
            inStream.putback(c);
        }
    
    //comments
    } else if(c == '-' && isdigit(inStream.peek())){
        setIsNegative(true);
        int returnNegVal = readInteger();
        token.setWholeNumber(returnNegVal);
        setIsNegative(false);
    }
    else if( reachedRelationalOperator(c)) {
        // put the relation operator back into the input stream so
        // we read the entire operator in a function
        // does NOT include ==
        inStream.putback(c);
        token.setRelationalOperator( readRelationalOperator() );
    }
    //comment parsing
    else if(c == '#'){
        while(inStream.get(c) && c != '\n')
            ;
        token.eol() = true;
        
    }
    //Check first quote to determine if string
    else if(c == '"')
    {
        token.setStringValue(readString());
    }
    // All else ifs from here are assigning symbol inside of token, implying
    // that there will be a whitespace behind the symbol 
    else if( c == '=' ){
        // Step 2
        // Here we check if it is == or just =
        // NOTE: will NOT check for ===
        if (inStream.peek() == '=')
        {
            std::string relOp;
            relOp += c;
            inStream.get(c);
            relOp += c;
            token.setRelationalOperator( relOp );
        }
        else 
        {
            token.symbol(c);
        }
    }
    else if( c == '{' || c == '}' || c == ',' || c == ':')
        token.symbol(c);
        
    else if( c == '+' || c == '-' || c == '*' || c == '/' || c == '%')
    {
        std::string extended;
        if(c == '/' && inStream.peek() == '/')
        {
            extended += c;
            inStream.get(c);
            extended += c;
            token.setExtendedOp(extended);
        }
        else
            token.symbol(c);
    }
        
    else if( c == ';' )
        token.symbol(c);
    else if( c == '(' || c == ')')
        token.symbol(c);
    else if(isalpha(c)) {  // an identifier?
        // put c back into the stream so we can read the entire name in a function.
        inStream.putback(c);
        token.setName( readName() );
        //std::cout << token.getName() << std::endl;
        if(token.isKeyword())
            token.setIsKeyword();
    } else {
        std::cout << "Unknown character in input. ->" << c << "<-" << std::endl;
        exit(1);
    }
    // token.print();
    // std::cout << ", ";
    // Vector of all tokens from the file we are reading
    token.setIndentSpace(spaceCounter);
    _tokens.push_back(token);
    return lastToken = token;
}

void Tokenizer::ungetToken() {
    ungottenToken = true;
}

// iter is a reference to the base address of the first element of _tokens(vector of tokens)
// Calls print() in token.cpp
void Tokenizer::printProcessedTokens() {
    for(auto iter = _tokens.begin(); iter != _tokens.end(); ++iter) {
        iter->print();
        std::cout << std::endl;
    }
}
