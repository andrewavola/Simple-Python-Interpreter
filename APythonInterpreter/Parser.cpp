//
// Created by Ali Kooshesh on 4/4/23.
//


#include<vector>
#include<iostream>

#include "Token.hpp"
#include "Parser.hpp"
#include "Statements.hpp"

// Parser functions


// Checks all tokens up to a certain point in the execution and then exits
void Parser::die(std::string where, std::string message, Token &token) {
    std::cout << where << " " << message << std::endl;
    token.print();
    std::cout << std::endl;
    std::cout << "\nThe following is a list of tokens that have been identified up to this point.\n";
    tokenizer.printProcessedTokens();
    exit(1);
}

Statements *Parser::statements() {
    // This function is called when we KNOW that we are about to parse
    // a series of assignment statements.

    // This function parses the grammar rules:

    // <statement> -> <assignStatement> <statement>
    // <statement> -> Epsilon


    Statements *stmts = new Statements();
    Token tok = tokenizer.getToken();

    //isName returns a boolean**
    while (tok.isName() && !tok.isKeyword()) {
        tokenizer.ungetToken();
        AssignmentStatement *assignStmt = assignStatement();
        stmts->addStatement(assignStmt);
        tok = tokenizer.getToken();
    }
    while (tok.isName() && tok.isKeyword()){
        if(tok.getName() == "print")
        {
            tokenizer.ungetToken();
            PrintStatement *printStmt = printStatement();
            stmts->addStatement(printStmt);
            tok = tokenizer.getToken();
        }
        else if(tok.getName() == "for")
        {

        }
        else
        {  
            std::cout << "Error recognizing keyword...";
            exit(1);
        }
        
        
    }
    tokenizer.ungetToken();
    return stmts;
}

PrintStatement *Parser::printStatement(){
    //Parses the following grammar rule
    //
    //<print-statement> -> print <id>
    Token varName = tokenizer.getToken();
    if(!varName.isName() || !varName.isKeyword())
        die("Parser::printStatement", "Expected a name token, instead got", varName);

    ExprNode *rightHandSideExpr = primary();
    return new PrintStatement(rightHandSideExpr);
    

}


AssignmentStatement *Parser::assignStatement() {
    // Parses the following grammar rule
    //
    // <assign-stmtement> -> <id> = <expr>

    Token varName = tokenizer.getToken();
    if (!varName.isName())
        die("Parser::assignStatement", "Expected a name token, instead got", varName);

    Token assignOp = tokenizer.getToken();
    if (!assignOp.isAssignmentOperator())
        die("Parser::assignStatement", "Expected an equal sign, instead got", assignOp);

    ExprNode *rightHandSideExpr = relExpr();
    //Token tok = tokenizer.getToken();
    //if (!tok.isSemiColon())
       //die("Parser::assignStatement", "Expected a semicolon instead got", tok);

    return new AssignmentStatement(varName.getName(), rightHandSideExpr);
}


ExprNode *Parser::relExpr(){
    //This function parses the grammar rules:

    //<relExpr> -> <relTerm> {(==, !=) <relTerm>}
    //<relTerm> -> <relPrimary> {(>, >=, <, <=) <relPrimary>}
    // <relPrimary> -> <expr>
    // <expr> -> <term> {<add_op> <term>}
    // <add_op> -> + | - 
    
    ExprNode *left = relTerm();
    Token tok = tokenizer.getToken();
    while(tok.isEqualOperator() || tok.isNotEqualOperator())
    {
        std::cout << "i'm in equal town";
        InfixExprNode *p = new InfixExprNode(tok);
        p->left() = left;
        p->right() = relTerm();
        left = p;
        tok = tokenizer.getToken();
    }
    tokenizer.ungetToken();
    return left;

}
ExprNode *Parser::relTerm(){
    //This function parses the grammar rules:

    //<relTerm> -> <relPrimary> {(>, >=, <, <=) <relPrimary>}
    // <relPrimary> -> <expr>
    // <expr> -> <term> {<add_op> <term>}
    // <add_op> -> + | -

    ExprNode *left = relPrimary();
    Token tok = tokenizer.getToken();
    while(tok.isGreaterThanOperator() || tok.isGreaterThanOrEqualToOperator()
         || tok.isLessThanOperator() || tok.isLessThanOrEqualToOperator())
    {
        InfixExprNode *p = new InfixExprNode(tok);
        p->left() = left;
        p->right() = relPrimary();
        left = p;
        tok = tokenizer.getToken();

    }
    tokenizer.ungetToken();
    return left;
}
// This is going to reference expr()
ExprNode *Parser::relPrimary(){
    // This function parses the grammar rules:
    // <relPrimary> -> <expr>
    // <expr> -> <term> {<add_op> <term>}
    // <add_op> -> + | -

    ExprNode *left = expr();
    return left;

}

ExprNode *Parser::expr() {
    // This function parses the grammar rules:

    // <expr> -> <term> { <add_op> <term> }
    // <add_op> -> + | -

    // However, it makes the <add_op> left associative.

    ExprNode *left = term();
    Token tok = tokenizer.getToken();
    while (tok.isAdditionOperator() || tok.isSubtractionOperator()) {
        InfixExprNode *p = new InfixExprNode(tok);
        p->left() = left;
        p->right() = term();
        left = p;
        tok = tokenizer.getToken();
    }
    tokenizer.ungetToken();
    return left;
}


ExprNode *Parser::term() {
    // This function parses the grammar rules:

    // <term> -> <primary> { <mult_op> <primary> }
    // <mult_op> -> * | / | %

    // However, the implementation makes the <mult-op> left associate.
    ExprNode *left = primary();
    Token tok = tokenizer.getToken();

    while (tok.isMultiplicationOperator() || tok.isDivisionOperator() || tok.isModuloOperator()) {
        InfixExprNode *p = new InfixExprNode(tok);
        p->left() = left;
        p->right() = primary();
        left = p;
        tok = tokenizer.getToken();
    }
    tokenizer.ungetToken();
    return left;
}

ExprNode *Parser::primary() {
    // This function parses the grammar rules:

    // <primary> -> [0-9]+
    // <primary> -> [_a-zA-Z]+
    // <primary> -> (<expr>)

    Token tok = tokenizer.getToken();

    if (tok.isWholeNumber() )
        return new WholeNumber(tok);
    else if( tok.isName() )
        return new Variable(tok);
    else if (tok.isOpenParen()) {
        ExprNode *p = relExpr();
        Token token = tokenizer.getToken();
        if (!token.isCloseParen())
            die("Parser::primary", "Expected close-parenthesis, instead got", token);
        return p;
    }
    die("Parser::primary", "Unexpected token", tok);

    return nullptr;  // Will not reach this statement!
}
