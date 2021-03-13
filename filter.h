#ifndef FILTER_H
#define FILTER_H

#include <map>
#include <QObject>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include "mainwindow.h"
#include "error.h"

using namespace std;

class Filter
{

public:

    map<int, QString> lines;

    Filter();
    void filter(QString filename);  // Sort the program according to line numbers
    void displayCode();

private:

    MainWindow* window;

    void filterLine(QString line);
    int getLineNumber(QString line);

};

#endif // FILTER_H
