#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <vector>
#include <memory>
#include "mainwindow.h"
#include "token.h"
#include "expr.h"
#include "stmt.h"
#include "utils.h"

using namespace std;

class Parser
{
public:

    map<int, StmtPtr> statements;

    Parser(map<int, shared_ptr<Tokens>>& tokens);
    void parse();

private:

    map<int, shared_ptr<Tokens>> tokens;
    map<int, shared_ptr<Tokens>>::const_iterator lineIt;
    vector<TokenPtr>::const_iterator tokenIt;
    MainWindow* window;

    void parseLine();
    void printLineAST(int lineNumber);

    StmtPtr statement();
    StmtPtr remark();
    StmtPtr let();
    StmtPtr print();
    StmtPtr input();
    StmtPtr goTo();
    StmtPtr ifThen();
    StmtPtr end();
    StmtPtr error();

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
