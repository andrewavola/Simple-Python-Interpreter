//
// Created by Andrew Avola
// 

#include <iostream>
#include "Types.hpp"

// //TypeDescriptor ===============================================
//Constructors


TypeDescriptor::TypeDescriptor(){}
TypeDescriptor::TypeDescriptor(types dType): _type{dType}{}


// IntegerTypeDescriptor ========================================

//Constructors

IntegerTypeDescriptor::IntegerTypeDescriptor(int value, types dataType): _val{value}, _dataType{dataType}, TypeDescriptor(_dataType){}
IntegerTypeDescriptor::IntegerTypeDescriptor(std::string varName): _varName{varName} {}

std::string &IntegerTypeDescriptor::name(){
    return _varName;
}


void IntegerTypeDescriptor::print(){
    std::cout << name() << " " << returnVal() << std::endl;
}

// ===================================================================


// BoolTypeDescriptor ========================================

//Constructors
BoolTypeDescriptor::BoolTypeDescriptor(bool value, types dataType): _val{value}, _dataType{dataType}, TypeDescriptor{_dataType} {}
BoolTypeDescriptor::BoolTypeDescriptor(std::string boolName): _boolName{boolName}{}

std::string &BoolTypeDescriptor::name(){
    return _boolName;
}


void BoolTypeDescriptor::print(){
    std::cout << name() << " " << returnVal() << std::endl;
}

// ===================================================================

//DoubleTypeDescriptor ========================================

//Constructors
DoubleTypeDescriptor::DoubleTypeDescriptor(double value, types dataType): _val{value}, _dataType{dataType}, TypeDescriptor{_dataType}{}
DoubleTypeDescriptor::DoubleTypeDescriptor(std::string doubleName): _doubleName{doubleName}{}   

std::string &DoubleTypeDescriptor::name(){
    return _doubleName;
}



void DoubleTypeDescriptor::print(){
    std::cout << name() << " " << returnVal() << std::endl;
}

// ===================================================================

//StringTypeDescriptor

//Constructors
StringTypeDescriptor::StringTypeDescriptor(std::string value, types dataType):  _val{value}, _dataType{dataType}, TypeDescriptor{_dataType}{}
StringTypeDescriptor::StringTypeDescriptor(std::string stringName): _stringName{stringName} {}

std::string &StringTypeDescriptor::name(){
    return _stringName;
}



void StringTypeDescriptor::print(){
    std::cout << name() << " " << returnVal() << std::endl;
}

// ===================================================================