//
// Created by Ali Kooshesh on 4/4/23.
//

#ifndef APYTHONINTERPRETER_SYMTAB_HPP
#define APYTHONINTERPRETER_SYMTAB_HPP

#include <string>
#include <map>
#include "Types.hpp"
// This is a flat and integer-based symbol table. It allows for variables to be
// initialized, determines if a give variable has been defined or not, and if
// a variable has been defined, it returns its value.

class SymTab {
public:
    void setValueFor(const std::string &vName, TypeDescriptor *temp);
    
    bool isDefined(const std::string &vName);
    TypeDescriptor* getValueFor(const std::string &vName);
    void print();

private:
    std::map<std::string, TypeDescriptor *> symTab;
    bool debug = false;
};


#endif //APYTHONINTERPRETER_SYMTAB_HPP
