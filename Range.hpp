//
// Created by Amit and Andrew on 5/3/2023
// 
//MAKE SURE TO UPDATE MAKEFILE

#ifndef APYTHONINTERPRETER_RANGE_HPP
#define APYTHONINTERPRETER_RANGE_HPP

#include <iostream>
#include "SymTab.hpp"
class Range {
public:
    Range(int _rangeValue);  // set initValue to zero and stepValue to 1.
    Range(int _initValue, int _rangeValue);  // set stepVlaue to 1.
    Range(int _initValue, int _rangeValue, int _stepVaue);

    int &getInitVal(){return initValue;}
    void setLookupVal(std::string value){lookUpVal = value;} //sets iterator name
    std::string getLookupVal(){return lookUpVal;}
    bool condition(SymTab &symTab); // should we iterate?
    int next();       // the value to be assigned to the loop counter.

private:
      int initValue, stepValue, rangeValue;
      std::string lookUpVal;
};



#endif