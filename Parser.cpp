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
    int indentSpace = 0;
    //Takes care of EOL before our statements
    continueThroughEOL();
    
    Token tok = tokenizer.getToken();
    
    if(tok.getIsIndent() || tok.getIsOutdent()){
        indentSpace = tok.getIndentSpace();
        tok = tokenizer.getToken();
    }
    
    // std::cout << tok.getIndentSpace() << " , " << tokenizer.getIndentStack().top();
    while (tok.isName() && indentSpace == tokenizer.getIndentStack().top())
    {
        
        if (tok.isKeyword() && tok.getName() == "print" )
        {
            tokenizer.ungetToken();
            PrintStatement *printStmt = printStatement();
            stmts->addStatement(printStmt);
            continueThroughEOL();
            tok = tokenizer.getToken();
            indentSpace = tok.getIndentSpace();
            if(indentSpace != tokenizer.getIndentStack().top()){
                tokenizer.getIndentStack().pop();
                break;
            }
        }
        else if(tok.isKeyword() && tok.getName() == "for" )
        {
            tokenizer.ungetToken();
            ForStatement *forStmt = forStatement();
            stmts->addStatement(forStmt);
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
    
    if(!(indentSpace == tokenizer.getIndentStack().top())){
        std::cout << "One or more lines does not match in indentation level..." << std::endl;
        exit(1);
    }
        
    tokenizer.ungetToken();
    return stmts;
}

//Used to check for EOL's until EOF to avoid erroring out in main
void Parser::continueThroughEOL()
{
    Token placeholder = tokenizer.getToken();
    
    while(!placeholder.eof() && !placeholder.isName() && !(placeholder.symbol() > 0) && !(placeholder.getIsIndent() || placeholder.getIsOutdent())){
        
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

Range *Parser::initRange(const std::string varName){

    
    Range *rng;
    Token getNextToken;
    
    
    getNextToken = tokenizer.getToken();
    
    if(!getNextToken.isOpenParen())
         die("ForStatement ->", "Expected OPEN PARENTH, instead got", getNextToken);

    Token parameterOne = tokenizer.getToken();
    if(!parameterOne.isWholeNumber())
         die("ForStatement ->", "Expected FIRST argument to RANGE function, instead got", parameterOne);
    
    getNextToken = tokenizer.getToken();

    //If we have only one argument
    if(getNextToken.isCloseParen()){
         rng = new Range(parameterOne.getWholeNumber());
         rng->setLookupVal(varName);
         return rng;
    }   

    //If we have two arguments
    else if(getNextToken.isComma()){
        Token parameterTwo =  tokenizer.getToken();
        if(!parameterTwo.isWholeNumber())
            die("ForStatement ->", "Expected SECOND argument to RANGE function, instead got", parameterTwo);
        getNextToken = tokenizer.getToken();
        if(getNextToken.isCloseParen()){
            rng = new Range(parameterOne.getWholeNumber(), parameterTwo.getWholeNumber());
            rng->setLookupVal(varName);
            return rng;
        }
        else if(getNextToken.isComma()){
            //If we have three arguments
            Token parameterThree = tokenizer.getToken();
            if(!parameterThree.isWholeNumber())
                 die("ForStatement ->", "Expected THIRD argument to RANGE function, instead got", parameterThree);
            getNextToken = tokenizer.getToken();
            if(!getNextToken.isCloseParen())
                 die("ForStatement ->", "Expected Closed-Parenth to RANGE function, instead got", getNextToken);
            rng = new Range(parameterOne.getWholeNumber(), parameterTwo.getWholeNumber(), parameterThree.getWholeNumber());
            rng->setLookupVal(varName);
            return rng;
                
        }
        else 
            die("ForStatement ->", "Expected Closed-Paren or Comma, instead got", getNextToken);

    }
    else
        die("ForStatement ->", "Expected Closed-Paren or Comma, instead got", getNextToken);

    return rng;

}
ForStatement *Parser::forStatement(){
        
    Token forTok = tokenizer.getToken();
    if(!forTok.isName() || !forTok.isKeyword())
        die("ForStatement ->", "Expected FOR keyword, instead got", forTok);

    Statements *forLoopStatements;
    Range *rng;
    
    
    Token varName = tokenizer.getToken();
    if(!varName.isName())
        die("ForStatement ->", "Expected NAME token, instead got", varName);
    
    Token checkForIn = tokenizer.getToken();
    if(!checkForIn.isKeywordIn())
         die("ForStatement ->", "Expected NAME-IN token, instead got", checkForIn);

    Token checkForRange = tokenizer.getToken();
    if(!checkForRange.isKeywordRange())
         die("ForStatement ->", "Expected NAME-RANGE, instead got", checkForRange);
        
    
    //Initialize range class object
    rng = initRange(varName.getName());

    Token getNextToken = tokenizer.getToken();
    if(!getNextToken.isColon())
         die("ForStatement ->", "Expected Colon, instead got", getNextToken);
    
    continueThroughEOL();

    //We know we found an open bracket to start
    getNextToken = tokenizer.getToken();
    if(!getNextToken.getIsIndent())
        die("ForStatement ->", "Expected INDENT for a statement, instead got", getNextToken);
    tokenizer.ungetToken();
    // if(getNextToken.isName())
    //     tokenizer.ungetToken();
        
    
    forLoopStatements = statements();

    // while(getNextToken.getIndentSpace() == tokenizer.getIndentStack().top()){
    //         forLoopStatements = statements();
    //         getNextToken = tokenizer.getToken();
            
    //         if(getNextToken.eol()){
    //             continueThroughEOL();
    //             getNextToken = tokenizer.getToken();
                
    //         }
           
    // }

    return new ForStatement(forLoopStatements, rng);

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
