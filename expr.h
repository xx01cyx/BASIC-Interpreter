#ifndef EXPR_H
#define EXPR_H

#include <QString>
#include <memory>
#include <math.h>
#include <QHash>
#include "token.h"
#include "error.h"
//#include "environment.h"

using namespace std;

typedef QHash<QString, int> Environment;

class Expr
{
public:
    Expr() {}
    virtual int evaluate(Environment& environment) = 0;
};

typedef shared_ptr<Expr> ExprPtr;


class ConstantExpr : public Expr
{
public:

    int value;

    ConstantExpr(int v) : value(v) {}

    int evaluate(Environment& environment) override { return value; }

};

class IdentifierExpr : public Expr
{
public:
    QString name;

    IdentifierExpr(QString n) : name(n) {}

    int evaluate(Environment& environment) override {
        if (environment.find(name) != environment.end())
            return environment[name];
        throw RunTimeError("The variable " + name + " does not exist.");
    }

};

class UnaryExpr : public Expr
{
public:
    ExprPtr right;

    UnaryExpr(ExprPtr r) : right(r) {}

    int evaluate(Environment& environment) override {
        return -right->evaluate(environment);
    }

};


class CompoundExpr : public Expr
{
public:
    ExprPtr left;
    TokenType op;
    ExprPtr right;

    CompoundExpr(ExprPtr l, TokenType op, ExprPtr r)
        : left(l), op(op), right(r) {}

    int evaluate(Environment& environment) override {
        switch(op) {
        case PLUS: return left->evaluate(environment) + right->evaluate(environment);
        case MINUS: return left->evaluate(environment) - right->evaluate(environment);
        case MULTIPLY: return left->evaluate(environment) * right->evaluate(environment);
        case DEVIDE: return left->evaluate(environment) / right->evaluate(environment);
        case POWER: return pow(left->evaluate(environment), right->evaluate(environment));
        }
    }
};

class GroupingExpr : public Expr
{
public:
    ExprPtr expr;

    GroupingExpr(ExprPtr e) : expr(e) {}

    int evaluate(Environment& environment) override {
        return expr->evaluate(environment);
    }
};


#endif // EXPR_H
