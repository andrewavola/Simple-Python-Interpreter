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

    while (tok.isName())
    {
        if (tok.isKeyword() && tok.getName() == "print" )
        {
            tokenizer.ungetToken();
            PrintStatement *printStmt = printStatement();
            stmts->addStatement(printStmt);
            tok = tokenizer.getToken();
        }
        else if(tok.isKeyword() && tok.getName() == "for" )
        {
            ForStatement *forStmt = forStatement();
            stmts->addStatement(forStmt);
            tok = tokenizer.getToken();
        }
        else if(!tok.isKeyword())
        {
            tokenizer.ungetToken();
            AssignmentStatement *assignStmt = assignStatement();
            stmts->addStatement(assignStmt);
            tok = tokenizer.getToken();
        }
        else
        {
            std::cout << "Error recognizing keyword or ID...";
            exit(1);
        }
    }
    /*
    //isName returns a boolean**
    while (tok.isName() && !tok.isKeyword()) {
        //tok.print(); std::cout << "\n";
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
            //tokenizer.ungetToken();
            ForStatement *forStmt = forStatement();
            stmts->addStatement(forStmt);
            tok = tokenizer.getToken();
        }   
        else
        {  
            std::cout << "Error recognizing keyword...";
            exit(1);
        }
        */
    
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

ForStatement *Parser::forStatement(){
        
    Token openParen = tokenizer.getToken();
    std::vector<Statements*> forLoopStatements;
    AssignmentStatement *lhs;
    AssignmentStatement *rhs; 
    ExprNode *mid;

    if(openParen.isOpenParen())
        lhs = assignStatement();
    else
        die("ForStatement ->", "Expected open-parenthesis, instead got", openParen);
    
    // We found our initializing assignment statement of our for loop successfuly, and
    // now need to determine if proper syntax follows our first assignment statement. If it 
    // does, then we begin to find our rel-expression.
    Token isFirstSemi = tokenizer.getToken();
    if(isFirstSemi.isSemiColon())
        mid = relExpr();
    else
        die("ForStatement ->", "Expected semi-colon, instead got", isFirstSemi);

    Token isSecondSemi = tokenizer.getToken();
    if(isSecondSemi.isSemiColon())
        rhs = assignStatement();
    else
        die("ForStatement ->", "Expected semi-colon, instead got", isSecondSemi);

    Token isClosedParenth = tokenizer.getToken();
    if(isClosedParenth.isCloseParen())
    {
        Token isOpenBrack = tokenizer.getToken();
        Token isClosedBrack;
        if(isOpenBrack.isOpenBrack())
        {
            isClosedBrack = tokenizer.getToken();
            while(!isClosedBrack.isClosedBrack())
            {
                tokenizer.ungetToken();
                Statements *statementsPtr = statements();
                forLoopStatements.push_back(statementsPtr);
                isClosedBrack = tokenizer.getToken();
            }
        }
        else    
            die("ForStatement ->", "Expected closed-bracket, instead got", isClosedBrack);
    }
    else
        die("ForStatement ->", "Expected closed parenthesis, instead got", isClosedParenth);
    
    return new ForStatement(lhs, mid, rhs, forLoopStatements);

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
