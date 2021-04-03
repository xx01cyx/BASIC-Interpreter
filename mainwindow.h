#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QEventLoop>
#include <QKeyEvent>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    QEventLoop loop;

    static MainWindow* getInstance();

    QString openFile();
    void appendCode(QString code);
    bool checkRun();
    void clearResult();
    void printResult(QString result);
    void clearText();
    void helpInfo();
    QString getInput();
    void printAST(int indent, QString content);
    int getProgramStatus();
    void setProgramStatus(int status);
    void setCmdPrompt();
    void clearCmdPrompt();

private slots:
    void on_pushButton_load_clicked();
    void on_pushButton_run_clicked();
    void on_pushButton_save_clicked();
    void on_pushButton_clear_clicked();
    void on_lineEdit_command_returnPressed();
    void on_lineEdit_command_cursorPositionChanged(int oldPos, int newPos);
    void on_lineEdit_command_textChanged(const QString &arg1);

signals:
    void load();
    void run();
    void clear();
    void command(QString);
    void input(QString);

private:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void saveFile();
    void waitForInput();

    static MainWindow* window;
    int programStatus;

    Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H
