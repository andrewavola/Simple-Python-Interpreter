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
    
    //Takes care of EOL before our statements
    continueThroughEOL();
    Token tok = tokenizer.getToken();
    
    while (tok.isName())
    {
        if (tok.isKeyword() && tok.getName() == "print" )
        {
            tokenizer.ungetToken();
            PrintStatement *printStmt = printStatement();
            stmts->addStatement(printStmt);
            continueThroughEOL();
            tok = tokenizer.getToken();
        
        }
        else if(tok.isKeyword() && tok.getName() == "for" )
        {
            tokenizer.ungetToken();
            ForStatement *forStmt = forStatement();
            stmts->addStatement(forStmt);
            
            tok = tokenizer.getToken();
            continueThroughEOL();
            tok = tokenizer.getToken();
        }
        else if(!tok.isKeyword())
        {
            tokenizer.ungetToken();
            AssignmentStatement *assignStmt = assignStatement();
            stmts->addStatement(assignStmt);
            continueThroughEOL();
            tok = tokenizer.getToken();
            
        }
        else
        {
            std::cout << "Error recognizing keyword or ID...";
            exit(1);
        }
    }
    
    tokenizer.ungetToken();
    return stmts;
}

//Used to check for EOL's until EOF to avoid erroring out in main
void Parser::continueThroughEOL()
{
    Token placeholder = tokenizer.getToken();
    
    while(!placeholder.eof() && !placeholder.isName() && !(placeholder.symbol() > 0)){
        
         placeholder = tokenizer.getToken();
    }
       
       
    tokenizer.ungetToken();
    
}

PrintStatement *Parser::printStatement(){
    //Parses the following grammar rule
    //
    //<print-statement> -> print <id>

    
    std::vector<ExprNode *> expressions;
    ExprNode *temp;
    Token varName = tokenizer.getToken();
    if(!varName.isName() || !varName.isKeyword())
        die("Parser::printStatement", "Expected a print token, instead got", varName);
    Token firstParenth = tokenizer.getToken();
    if(!firstParenth.isOpenParen())
        die("Parser::printStatement", "Expected a open-parenthesis, instead got", firstParenth);

    //ID
    Token isComma;
    //Get first expression in the print statement
    ExprNode *firstExpr = relExpr();

    //loop for more expressions that are comma delimited
    isComma = tokenizer.getToken();
    
    if(isComma.isComma()){
        expressions.push_back(firstExpr);
        while(isComma.isComma()){
            temp = relExpr();
            expressions.push_back(temp);
            isComma = tokenizer.getToken();
        }
        tokenizer.ungetToken();
    }
    else
    {
        expressions.push_back(firstExpr);
        tokenizer.ungetToken();
    }
        
    

    Token closingParenth = tokenizer.getToken();
    if(!closingParenth.isCloseParen())
        die("Parser::printStatement", "Expected a closed-parenthesis, instead got", closingParenth);
    //Check EOL
    Token tok = tokenizer.getToken();
    if(!tok.eol())
    {
       die("Parser::printStatement", "Expected a new-line, instead got", tok);
    }

    return new PrintStatement(expressions);
    

}

ForStatement *Parser::forStatement(){
        
    Token forTok = tokenizer.getToken();
    if(!forTok.isName() || !forTok.isKeyword())
        die("ForStatement ->", "Expected FOR keyword, instead got", forTok);
    std::vector<Statements*> forLoopStatements;
    AssignmentStatement *lhs;
    AssignmentStatement *rhs; 
    ExprNode *mid;
    Token openParen = tokenizer.getToken();
    if(openParen.isOpenParen())
    {
        lhs = assignStatement();
    }
        
    else
        die("ForStatement ->", "Expected open-parenthesis, instead got", openParen);
    
    // We found our initializing assignment statement of our for loop successfuly, and
    // now need to determine if proper syntax follows our first assignment statement. If it 
    // does, then we begin to find our rel-expression.
    Token isFirstSemi = tokenizer.getToken();
    
    if(isFirstSemi.isSemiColon())
    {
        mid = relExpr();
    }
        
    else
        die("ForStatement ->", "Expected semi-colon, instead got", isFirstSemi);

    Token isSecondSemi = tokenizer.getToken();
    // std::cout << "IsSecondSemi: ";
    // isSecondSemi.print();
    // exit(1);
    if(isSecondSemi.eol())
    {
        continueThroughEOL();
        isSecondSemi = tokenizer.getToken();
    }
       
    if(isSecondSemi.isSemiColon())
    {
        // isSecondSemi = tokenizer.getToken();
        rhs = assignStatement();
        // rhs->print();
        // exit(1);     
    }
    else
        die("ForStatement ->", "Expected semi-colon, instead got", isSecondSemi);

    Token isClosedParenth = tokenizer.getToken();
    if(isClosedParenth.eol()){
        continueThroughEOL();
        isClosedParenth = tokenizer.getToken();
    }
    
    if(isClosedParenth.isCloseParen())
    {
        Token isOpenBrack = tokenizer.getToken();
        Token isClosedBrack;
        if(isOpenBrack.isOpenBrack())
        {
            isClosedBrack = tokenizer.getToken();
            
            //Check for EOL
            if(isClosedBrack.eol())
            {
                continueThroughEOL();
                isClosedBrack = tokenizer.getToken();
            }
            //vars
            else
                tokenizer.ungetToken();

            if(isClosedBrack.isName())
                tokenizer.ungetToken();

            while(!isClosedBrack.isClosedBrack())
            {
                
                // tokenizer.ungetToken();
                Statements *statementsPtr = statements();
                forLoopStatements.push_back(statementsPtr);
                // statementsPtr->print();
                isClosedBrack = tokenizer.getToken();
                if(isClosedBrack.eol()){
                    continueThroughEOL();
                    isClosedBrack = tokenizer.getToken();
                }
                
            }
            // Token getAfterClosedBrack = tokenizer.getToken();
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
    
    if(varName.eol())
    {
        continueThroughEOL();
        varName = tokenizer.getToken();
    }
    
    
    if (!varName.isName())
        die("Parser::assignStatement", "Expected a name token, instead got", varName);
    
    
    Token assignOp = tokenizer.getToken();
    
    if (!assignOp.isAssignmentOperator())
        die("Parser::assignStatement", "Expected an equal sign, instead got", assignOp);

    ExprNode *rightHandSideExpr = relExpr();
    
    // Token tok = tokenizer.getToken();
    
    // if (!tok.eol())
    // {
    //     die("Parser::assignStatement", "Expected a new-line instead got", tok);
    // }
        
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
    
    ExprNode *left = relPrimary();
    Token tok = tokenizer.getToken();
    while(tok.isEqualOperator() || tok.isNotEqualSecondary() || tok.isNotEqualOperator()
         || tok.isLessThanOperator() || tok.isGreaterThanOperator() || tok.isLessThanOrEqualToOperator()
         || tok.isGreaterThanOrEqualToOperator())
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
// ExprNode *Parser::relTerm(){
//     //This function parses the grammar rules:

//     //<relTerm> -> <relPrimary> {(>, >=, <, <=) <relPrimary>}
//     // <relPrimary> -> <expr>
//     // <expr> -> <term> {<add_op> <term>}
//     // <add_op> -> + | -

//     ExprNode *left = relPrimary();
//     Token tok = tokenizer.getToken();
//     while(tok.isGreaterThanOperator() || tok.isGreaterThanOrEqualToOperator()
//          || tok.isLessThanOperator() || tok.isLessThanOrEqualToOperator())
//     {
//         InfixExprNode *p = new InfixExprNode(tok);
//         p->left() = left;
//         p->right() = relPrimary();
//         left = p;
//         tok = tokenizer.getToken();

//     }
//     tokenizer.ungetToken();
//     return left;
// }

// This is going to reference expr()
ExprNode *Parser::relPrimary(){
    // This function parses the grammar rules:
    // <relPrimary> -> <expr>
    // <expr> -> <term> {<add_op> <term>}
    // <add_op> -> + | -

    ExprNode *left = arith_expr();
    return left;

}

ExprNode *Parser::arith_expr() {
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

    while (tok.isMultiplicationOperator() || tok.isDivisionOperator() || tok.isModuloOperator() || tok.isExtendedOpFloor()) {
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
    else if(tok.isDoubleNumber())
        return new DoubleNumber(tok);
    else if(tok.isString())
        return new StringValue(tok);
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
