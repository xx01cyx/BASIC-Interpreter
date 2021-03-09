#ifndef STMT_H
#define STMT_H

#include <memory>
#include "token.h"
#include "expr.h"

using namespace std;

class Stmt
{
public:

    Stmt() {}

    // virtual void execute() = 0;

};

typedef shared_ptr<Stmt> StmtPtr;


class RemarkStmt : public Stmt
{
public:

    TokenPtr remark;

    RemarkStmt(TokenPtr remark) : remark(remark) {}

    // void execute() override {}

};

class LetStmt : public Stmt
{
public:

    TokenPtr variable;
    ExprPtr initializer;

    LetStmt(TokenPtr variable, ExprPtr initializer)
        : variable(variable), initializer(initializer) {}

};


class PrintStmt : public Stmt
{
public:

    ExprPtr expression;

    PrintStmt(ExprPtr expr) : expression(expr) {}

};

class InputStmt : public Stmt
{
public:

    TokenPtr variable;

    InputStmt(TokenPtr variable) : variable(variable) {}

};

class GotoStmt : public Stmt
{
public:

    TokenPtr line;

    GotoStmt(TokenPtr line) : line(line) {}

};

class IfStmt : public Stmt
{
public:

    ExprPtr left;
    TokenType comparison;
    ExprPtr right;
    TokenPtr line;

    IfStmt(ExprPtr left, TokenType comparison, ExprPtr right, TokenPtr line)
        : left(left), comparison(comparison), right(right), line(line) {}

};

class EndStmt : public Stmt
{
public:

    EndStmt() {}

};

#endif // STMT_H
