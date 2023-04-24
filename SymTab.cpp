//
// Created by Ali Kooshesh on 4/4/23.
//

#include <iostream>
#include "SymTab.hpp"


void SymTab::setValueFor(const std::string &vName,  TypeDescriptor *temp) {
    // Define a variable by setting its initial value.
    // NumberDescriptor *test = new NumberDescriptor(desc->type());
    // if(debug)
    //     std::cout << vName << " <- " << parsedValue << std::endl;
    if(temp->getType() == TypeDescriptor::INTEGER)
    {
        int temporary = dynamic_cast<IntegerTypeDescriptor *>(temp)->returnVal();
        symTab[vName] = new IntegerTypeDescriptor(temporary, TypeDescriptor::INTEGER);
    }
    else if(temp->getType() == TypeDescriptor::DOUBLE)
    {
        double temporary = dynamic_cast<DoubleTypeDescriptor *>(temp)->returnVal();
        symTab[vName] = new DoubleTypeDescriptor(temporary, TypeDescriptor::DOUBLE);
    }
    else if(temp->getType() == TypeDescriptor::BOOL)
    {
        bool temporary = dynamic_cast<BoolTypeDescriptor *>(temp)->returnVal();
        symTab[vName] = new BoolTypeDescriptor(temporary, TypeDescriptor::BOOL);
    }
    else if(temp->getType() == TypeDescriptor::STRING)
    {
        std::string temporary = dynamic_cast<StringTypeDescriptor *>(temp)->returnVal();
        symTab[vName] = new StringTypeDescriptor(temporary, TypeDescriptor::STRING);
    }
        
    else
    {
        std::cout << "Parsed datatype was not allowed" << std::endl;
        exit(1);
    }
        
    //symTab[vName] = value;
}

bool SymTab::isDefined(const std::string &vName) {
    return symTab.find(vName) != symTab.end();
}

TypeDescriptor *SymTab::getValueFor(const std::string &vName) {
    if( ! isDefined(vName)) {
        std::cout << "SymTab::getValueFor: " << vName << " has not been defined.\n";
        exit(1);
    }
    if(debug)
        std::cout << "SymTab::getValueFor: " << vName << " contains " << symTab.find(vName)->second << std::endl;
    return symTab.find(vName)->second;
}

void SymTab::print() {
    for(auto [var, value] : symTab )
    {
        if(value->getType() == TypeDescriptor::INTEGER)
        {
            IntegerTypeDescriptor *placeHolder = dynamic_cast<IntegerTypeDescriptor *>(value);
            std::cout << var << " = " << placeHolder->returnVal() << std::endl;
        }
        else if(value->getType() == TypeDescriptor::DOUBLE)
        {
            DoubleTypeDescriptor *placeHolder = dynamic_cast<DoubleTypeDescriptor *>(value);
            std::cout << var << " = " << placeHolder->returnVal() << std::endl;
        }
        else if(value->getType() == TypeDescriptor::BOOL)
        {
            BoolTypeDescriptor *placeHolder = dynamic_cast<BoolTypeDescriptor *>(value);
            std::cout << var << " = " << placeHolder->returnVal() << std::endl;
        }
        else if(value->getType() == TypeDescriptor::STRING)
        {
            StringTypeDescriptor *placeHolder = dynamic_cast<StringTypeDescriptor *>(value);
            std::cout << var << " = " << placeHolder->returnVal() << std::endl;
        }
           
    }
    
        
}

