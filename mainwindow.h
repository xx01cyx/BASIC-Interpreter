#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    static MainWindow* getInstance();

    QString openFile();
    void appendCode(QString code);
    bool checkRun();
    void printResult(QString result);
    void clearText();
    void helpInfo();

private slots:
    void on_pushButton_load_clicked();
    void on_pushButton_run_clicked();
    void on_pushButton_clear_clicked();
    void on_lineEdit_command_returnPressed();

signals:
    void load();
    void run();
    void clear();
    void command(QString);

private:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    static MainWindow* window;

    Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H
