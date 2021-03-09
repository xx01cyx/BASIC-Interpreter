#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <vector>
#include <memory>
#include "token.h"
#include "expr.h"
#include "stmt.h"
#include "error.h"

using namespace std;

class Parser
{
public:

    map<int, StmtPtr> statements;

    Parser(map<int, shared_ptr<Tokens>>& tokens);
    void parse();

private:

    map<int, shared_ptr<Tokens>> tokens;
    map<int, shared_ptr<Tokens>>::const_iterator mapIt;
    vector<TokenPtr>::const_iterator tokenIt;

    void parseLine();

    StmtPtr statement();
    StmtPtr remark();
    StmtPtr let();
    StmtPtr print();
    StmtPtr input();
    StmtPtr goTo();
    StmtPtr ifThen();
    StmtPtr end();

    ExprPtr expression();
    ExprPtr term();
    ExprPtr factor();
    ExprPtr unaryPower();
    ExprPtr power();
    ExprPtr unary();
    ExprPtr primary();

    bool match(TokenType type);
    bool endOfLine();
    void advance();
    TokenPtr previous();

};

#endif // PARSER_H
