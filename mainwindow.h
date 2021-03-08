#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    void displayCode();
    void readFile(QString);
    QString openFile();

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_load_clicked();
    void on_lineEdit_command_returnPressed();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
