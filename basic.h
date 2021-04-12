#ifndef BASIC_H
#define BASIC_H

#include <QObject>
#include <QFileDialog>
#include <QString>
#include "mainwindow.h"
#include "filter.h"
#include "scanner.h"
#include "parser.h"
#include "interpreter.h"
#include "utils.h"

class Basic : public QObject
{
    Q_OBJECT

private:
    MainWindow* window;
    Filter* filter;
    Scanner* scanner;
    Parser* parser;
    Interpreter* interpreter;

    void executeCmdProgram(StmtPtr stmt);

private slots:
    void receiveCmdProgram(QString);
    void loadProgram();
    void runProgram();
    void clearProgram();
    void enterCommand(QString);
    void getHelp();
    void quitInterpreter();

public:
    Environment global;

    Basic();
    ~Basic();
    void run();
};

#endif // BASIC_H
