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
    virtual int evaluate(Environment& global, Environment& local) = 0;
    virtual void generateAST(int indent) const = 0;

};

typedef shared_ptr<Expr> ExprPtr;


class ConstantExpr : public Expr
{
public:

    int value;

    ConstantExpr(int v) : value(v) {}

    int evaluate(Environment& global, Environment& local) override { return value; }

    void generateAST(int indent) const override {
        window->printAST(indent, QString::number(value));
    }
};

class IdentifierExpr : public Expr
{
public:
    QString name;

    IdentifierExpr(QString n) : name(n) {}

    int evaluate(Environment& global, Environment& local) override {
        if (local.find(name) != local.end())
            return local[name];
        if (global.find(name) != global.end())
            return global[name];
        throw RunTimeError("Variable `" + name + "` does not exist.");
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

    int evaluate(Environment& global, Environment& local) override {
        return -right->evaluate(global, local);
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

    int evaluate(Environment& global, Environment& local) override {

        int leftOp = left->evaluate(global, local);
        int rightOp = right->evaluate(global, local);

        switch(op) {
            case PLUS: return leftOp + rightOp;
            case MINUS: return leftOp - rightOp;
            case MULTIPLY: return leftOp * rightOp;
            case DEVIDE:
                if (!rightOp)
                    throw RunTimeError("Divisor cannot be 0.");
                return leftOp / rightOp;
            case POWER:
                if (!leftOp && !rightOp)
                    throw RunTimeError("Base number and exponent number cannot both be 0.");
                return pow(leftOp, rightOp);
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

    int evaluate(Environment& global, Environment& local) override {
        return expr->evaluate(global, local);
    }

    void generateAST(int indent) const override {
        expr->generateAST(indent);
    }
};

class ErrorExpr : public Expr
{
public:

    ErrorExpr() {}

    int evaluate(Environment& global, Environment& local) override {}

    void generateAST(int indent) const override {}
};


#endif // EXPR_H
