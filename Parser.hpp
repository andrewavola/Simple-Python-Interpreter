//
// Created by Ali Kooshesh on 4/4/23.
//

#ifndef APYTHONINTERPRETER_PARSER_HPP
#define APYTHONINTERPRETER_PARSER_HPP

#include "Token.hpp"
#include "Tokenizer.hpp"
#include "SymTab.hpp"
#include "ExprNode.hpp"
#include "Statements.hpp"

#include<vector>
#include<iostream>
#include<map>


class Parser {
public:
    Parser(Tokenizer &tokenizer) : tokenizer{tokenizer} {}

    Statements *statements();

    AssignmentStatement *assignStatement();
    PrintStatement  *printStatement();
    ForStatement *forStatement();

    void continueThroughEOL();
    ExprNode *arith_expr();
    ExprNode *term();
    ExprNode *primary();

    //Step 4:
    ExprNode *relExpr();
    ExprNode *relTerm();
    ExprNode *relPrimary();

    

    std::string id();

private:
    Tokenizer &tokenizer;

    void die(std::string where, std::string message, Token &token);

};


#endif //APYTHONINTERPRETER_PARSER_HPP
