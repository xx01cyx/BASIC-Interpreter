#ifndef FILTER_H
#define FILTER_H

#include <map>
#include <unordered_map>
#include <QObject>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include "mainwindow.h"
#include "utils.h"

using namespace std;

enum Command {
    RUN, LOAD, LIST, CLEAR, HELP, QUIT,
};

class Filter : public QObject
{
    Q_OBJECT

public:

    map<int, QString> lines;

    Filter();
    void filter(QString filename);  // Sort the program according to line numbers
    void displayCode();
    void filterCmd(QString cmd);
    void reset();

private slots:
    void executeCommand(Command command);

signals:
    void run();
    void load();
    void clear();
    void help();
    void quit();

private:

    MainWindow* window;
    unordered_map<QString, Command> commands;

    void filterLine(QString line);
    int getLineNumber(QString line, bool isProgram);

};

#endif // FILTER_H
