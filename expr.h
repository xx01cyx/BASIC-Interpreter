#ifndef EXPR_H
#define EXPR_H

#include <QString>
#include <memory>
#include <math.h>
#include "token.h"

using namespace std;

class Expr
{
public:
    Expr() {}
    virtual int evaluate() = 0;
};

typedef shared_ptr<Expr> ExprPtr;


class ConstantExpr : public Expr
{
public:

    int value;

    ConstantExpr(int v) : value(v) {}

    int evaluate() override { return value; }

};

class IdentifierExpr : public Expr
{
public:
    QString name;

    IdentifierExpr(QString n) : name(n) {}

    int evaluate() override { return 0; }

};

class UnaryExpr : public Expr
{
public:
    ExprPtr right;

    UnaryExpr(ExprPtr r) : right(r) {}

    int evaluate() override { return -right->evaluate(); }

};


class CompoundExpr : public Expr
{
public:
    ExprPtr left;
    TokenType op;
    ExprPtr right;

    CompoundExpr(ExprPtr l, TokenType op, ExprPtr r)
        : left(l), op(op), right(r) {}

    int evaluate() override {
        switch(op) {
        case PLUS: return left->evaluate() + right ->evaluate();
        case MINUS: return left->evaluate() - right ->evaluate();
        case MULTIPLY: return left->evaluate() * right ->evaluate();
        case DEVIDE: return left->evaluate() / right ->evaluate();
        case POWER: return pow(left->evaluate(), right->evaluate());
        }
    }
};


#endif // EXPR_H
