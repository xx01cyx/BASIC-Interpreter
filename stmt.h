#ifndef STMT_H
#define STMT_H

#include <memory>
#include <QHash>
#include <QString>
#include "mainwindow.h"
#include "token.h"
#include "expr.h"
#include "utils.h"

using namespace std;

typedef QHash<QString, int> Environment;

class Stmt
{
protected:

    MainWindow* window;

public:

    TokenType type;

    Stmt(TokenType type) : type(type) { window = MainWindow::getInstance(); }
    virtual void execute(Environment &environment, int& pc) = 0;
    virtual void generateAST() const = 0;

};

typedef shared_ptr<Stmt> StmtPtr;


class RemarkStmt : public Stmt
{
public:

    TokenPtr remark;

    RemarkStmt(TokenPtr remark) : Stmt(REM), remark(remark) {}

    void execute(Environment &environment, int& pc) override {}

    void generateAST() const override {
        window->printAST(1, remark->lexeme);
    }

};

class LetStmt : public Stmt
{
public:

    TokenPtr variable;
    ExprPtr initializer;

    LetStmt(TokenPtr variable, ExprPtr initializer)
        : Stmt(LET), variable(variable), initializer(initializer) {}

    void execute(Environment &environment, int& pc) override {
        environment[variable->lexeme] = initializer->evaluate(environment);
    }

    void generateAST() const override {
        window->printAST(1, variable->lexeme);
        initializer->generateAST(1);
    }
};


class PrintStmt : public Stmt
{
public:

    ExprPtr expression;

    PrintStmt(ExprPtr expr) : Stmt(PRINT), expression(expr) {}

    void execute(Environment &environment, int& pc) override {
        int result = expression->evaluate(environment);
        window->printResult(QString::number(result));
    }

    void generateAST() const override {
        expression->generateAST(1);
    }

};

class InputStmt : public Stmt
{
public:

    TokenPtr variable;

    InputStmt(TokenPtr variable) : Stmt(INPUT), variable(variable) {}

    // to be revised

    void execute(Environment &environment, int& pc) override {

    }

    void generateAST() const override {
        window->printAST(1, variable->lexeme);
    }

};

class GotoStmt : public Stmt
{
public:

    TokenPtr line;

    GotoStmt(TokenPtr line) : Stmt(GOTO), line(line) {}

    void execute(Environment &environment, int& pc) override {

        bool ok;
        int lineNumber = (line->lexeme).toInt(&ok, 0);
        if (!ok || lineNumber <= 0)
            throw RunTimeError("Invalid line number.");

        pc = lineNumber;
    }

    void generateAST() const override {
        window->printAST(1, line->lexeme);
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
        : Stmt(IF), left(left), comparison(comparison), right(right), line(line) {}

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

    void generateAST() const override {

        QString cmpStr;

        switch (comparison) {
            case LESS: cmpStr = "<"; break;
            case GREATER: cmpStr = ">"; break;
            case EQUAL: cmpStr = "=";
        }

        left->generateAST(1);
        window->printAST(1, cmpStr);
        right->generateAST(1);
        window->printAST(1, line->lexeme);
    }
};

class EndStmt : public Stmt
{
public:

    EndStmt() : Stmt(END) {}

    void execute(Environment &environment, int& pc) override {
        environment.clear();
        throw EndProgram();
    }

    void generateAST() const override {}

};


#endif // STMT_H
