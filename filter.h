#ifndef FILTER_H
#define FILTER_H

#include <map>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include "error.h"

using namespace std;

class Filter
{
public:
    map<int, QString> lines;

    Filter();

    void filterCode(QString line);
    int getLineNumber(QString line);


};

#endif // FILTER_H
