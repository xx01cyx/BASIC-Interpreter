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
#include "error.h"

class Basic : public QObject
{
    Q_OBJECT

private:
    MainWindow* window;
    Filter* filter;

private slots:
    void loadProgram();
    void runProgram();

public:
    Basic();
    ~Basic();

    void run();
};

#endif // BASIC_H
