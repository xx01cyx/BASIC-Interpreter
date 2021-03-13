#ifndef STMT_H
#define STMT_H

#include <memory>
#include <QHash>
#include <QString>
#include "mainwindow.h"
#include "token.h"
#include "expr.h"

using namespace std;

typedef QHash<QString, int> Environment;

class Stmt
{
protected:

    MainWindow* window;

public:

    Stmt() { window = MainWindow::getInstance(); }

    virtual void execute(Environment &environment, int& pc) = 0;

};

typedef shared_ptr<Stmt> StmtPtr;


class RemarkStmt : public Stmt
{
public:

    TokenPtr remark;

    RemarkStmt(TokenPtr remark) : remark(remark) {}

    void execute(Environment &environment, int& pc) override {}

};

class LetStmt : public Stmt
{
public:

    TokenPtr variable;
    ExprPtr initializer;

    LetStmt(TokenPtr variable, ExprPtr initializer)
        : variable(variable), initializer(initializer) {}

    void execute(Environment &environment, int& pc) override {
        environment[variable->lexeme] = initializer->evaluate(environment);
    }

};


class PrintStmt : public Stmt
{
public:

    ExprPtr expression;

    PrintStmt(ExprPtr expr) : expression(expr) {}


    // to be revised

    void execute(Environment &environment, int& pc) override {
//        qDebug() << expression->evaluate(environment) << Qt::endl;
        int result = expression->evaluate(environment);
        window->printResult(QString::number(result));
    }

};

class InputStmt : public Stmt
{
public:

    TokenPtr variable;

    InputStmt(TokenPtr variable) : variable(variable) {}

    // to be revised

    void execute(Environment &environment, int& pc) override {}

};

class GotoStmt : public Stmt
{
public:

    TokenPtr line;

    GotoStmt(TokenPtr line) : line(line) {}

    void execute(Environment &environment, int& pc) override {

        bool ok;
        int lineNumber = (line->lexeme).toInt(&ok, 0);
        if (!ok)
            throw RunTimeError("Invalid line number.");

        pc = lineNumber;
    }

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

    void execute(Environment &environment, int& pc) override {

        int valL = left->evaluate(environment), valR = right->evaluate(environment);

        bool condition;

        switch (comparison) {
        case LESS: condition = (valL < valR); break;
        case GREATER: condition = (valL > valR); break;
        case EQUAL: condition = (valL == valR);
        }

        if (condition) {
            StmtPtr goTo = make_shared<GotoStmt>(line);
            goTo->execute(environment, pc);
        }

    }

};

class EndStmt : public Stmt
{
public:

    EndStmt() {}

    void execute(Environment &environment, int& pc) override {
        environment.clear();
    }

};

#endif // STMT_H
