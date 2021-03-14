#ifndef EXPR_H
#define EXPR_H

#include <QString>
#include <memory>
#include <math.h>
#include <QHash>
#include "mainwindow.h"
#include "token.h"
#include "utils.h"

using namespace std;

typedef QHash<QString, int> Environment;

class Expr
{
protected:

    MainWindow* window;

public:

    Expr() { window = MainWindow::getInstance(); }
    virtual int evaluate(Environment& environment) = 0;
    virtual void generateAST(int indent) const = 0;

};

typedef shared_ptr<Expr> ExprPtr;


class ConstantExpr : public Expr
{
public:

    int value;

    ConstantExpr(int v) : value(v) {}

    int evaluate(Environment& environment) override { return value; }

    void generateAST(int indent) const override {
        window->printAST(indent, QString::number(value));
    }
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

    void generateAST(int indent) const override {
        window->printAST(indent, name);
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

    void generateAST(int indent) const override {
        window->printAST(indent, "-");
        right->generateAST(indent + 1);
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

    void generateAST(int indent) const override {

        QString opStr;

        switch(op) {
            case PLUS: opStr = "+"; break;
            case MINUS: opStr = "-"; break;
            case MULTIPLY: opStr = "*"; break;
            case DEVIDE: opStr = "/"; break;
            case POWER: opStr = "**"; break;
        }

        window->printAST(indent, opStr);
        left->generateAST(indent+1);
        right->generateAST(indent+1);

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

    void generateAST(int indent) const override {
        expr->generateAST(indent);
    }
};


#endif // EXPR_H
