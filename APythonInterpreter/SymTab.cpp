//
// Created by Ali Kooshesh on 4/4/23.
//

#include <iostream>
#include "SymTab.hpp"

void SymTab::setValueFor(const std::string &vName, int value) {
    // Define a variable by setting its initial value.
    if(debug)
        std::cout << vName << " <- " << value << std::endl;
    symTab[vName] = value;
}

bool SymTab::isDefined(const std::string &vName) {
    return symTab.find(vName) != symTab.end();
}

int SymTab::getValueFor(const std::string &vName) {
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
        std::cout << var << " = " << value << std::endl;
}

