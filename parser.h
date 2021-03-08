#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <vector>
#include <memory>
#include "token.h"
#include "expr.h"
#include "error.h"

using namespace std;

class Parser
{
public:

    map<int, ExprPtr> expressions;  // to be revised

    Parser(map<int, shared_ptr<Tokens>>& tokens);

private:

    map<int, shared_ptr<Tokens>> tokens;
    map<int, shared_ptr<Tokens>>::const_iterator mapIt;
    vector<TokenPtr>::const_iterator tokenIt;

    void parse();

    ExprPtr term();
    ExprPtr factor();
    ExprPtr unaryPower();
    ExprPtr power();
    ExprPtr unary();
    ExprPtr primary();

    bool match(TokenType type);
    void advance();
    TokenPtr previous();

};

#endif // PARSER_H
