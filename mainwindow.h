#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "filter.h"
#include "scanner.h"
#include "parser.h"
#include "interpreter.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    Filter* filter;

    void displayCode();
    void readFile(QString);
    QString openFile();

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_load_clicked();
    void on_lineEdit_command_returnPressed();

    void on_pushButton_run_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
