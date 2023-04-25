//
// Created by Andrew Avola
//

#ifndef APYTHONINTERPRETER_TYPES_HPP
#define APYTHONINTERPRETER_TYPES_HPP

#include<iostream>




class TypeDescriptor {
public: 
    TypeDescriptor();
    enum types {INTEGER, DOUBLE, BOOL, STRING};
    TypeDescriptor(types dType);
    
    types &getType(){return _type;}

    virtual std::string &name() = 0;
    virtual void print() = 0;

private:
    types _type;
};

class IntegerTypeDescriptor: public TypeDescriptor{
public:
    IntegerTypeDescriptor(int value, types dataType); //TypeDescriptor(_dataType){};
    IntegerTypeDescriptor(std::string varName);

    virtual std::string &name();

    int &returnVal(){return _val;}
    void returnVal(int setVal){_val = setVal;}
    virtual void print();


private:
    std::string _varName;
    int _val;

    
};

class BoolTypeDescriptor: public TypeDescriptor{
public:
    BoolTypeDescriptor(bool value, types dataType);// TypeDescriptor(_dataType){};
    BoolTypeDescriptor(std::string boolName);

    virtual std::string &name();
    bool  &returnVal(){return _val;}
    void returnVal(bool setVal){_val = setVal;}
    virtual void print();

private:
    std::string _boolName;
    bool  _val;

};

class DoubleTypeDescriptor: public TypeDescriptor{
public:
    DoubleTypeDescriptor(double value, types dataType);//TypeDescriptor(_dataType){};
    DoubleTypeDescriptor(std::string doubleName);

    virtual std::string &name();
    double &returnVal(){return _val;}
    void returnVal(double setVal){_val = setVal;}
    virtual void print();

private:
    std::string _doubleName;
    double _val;

};

class StringTypeDescriptor: public TypeDescriptor{
public: 
    StringTypeDescriptor(std::string value, types dataType);// TypeDescriptor(_dataType){};
    StringTypeDescriptor(std::string stringName);

    std::string &returnVal(){return _val;}
    void returnVal(std::string setVal){_val = setVal;}
    virtual std::string &name();
    virtual void print();

private:
    std::string _stringName;
    std::string _val;
};


#endif //APYTHONINTERPRETER_TYPES_HPP