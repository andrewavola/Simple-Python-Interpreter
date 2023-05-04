#include<iostream>
#include "Range.hpp"


Range::Range(int _rVal): initValue{0}, stepValue{1}, rangeValue{_rVal}{}

Range::Range(int _initVal, int _rVal): initValue{_initVal}, rangeValue{_rVal}, stepValue{1}{}

Range::Range(int _initVal, int _rVal, int _stepVal){
    initValue = _initVal;
    stepValue = _stepVal;
    rangeValue = _rVal;
    if(stepValue == 0){
        std::cout << "ValueError:range() arg 3 must not be zero\n";
        exit(1);
    }
}

bool Range::condition(SymTab &symTab){
    //change initValue to map lookup
    if(dynamic_cast<IntegerTypeDescriptor*>(symTab.getValueFor(getLookupVal()))->returnVal() == rangeValue)
        return false;
    return true;
}

int Range::next(){
    initValue = initValue + stepValue;
    return initValue;
}

