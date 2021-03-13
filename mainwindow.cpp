#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QApplication>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

MainWindow* MainWindow::window = nullptr;

MainWindow* MainWindow::getInstance()
{
    if (window == nullptr)
        window = new MainWindow();
    return window;
}

void MainWindow::on_pushButton_load_clicked()
{
    emit load();
}

void MainWindow::on_pushButton_run_clicked()
{
    emit run();
}

void MainWindow::on_lineEdit_command_returnPressed()
{
    QString code = ui->lineEdit_command->text();

    ui->lineEdit_command->clear();
}

QString MainWindow::openFile()
{
    QString filename =  QFileDialog::getOpenFileName(
          this,
          "Open Document",
          QDir::currentPath(),
          "All files (*.*) ;; Document files (*.doc *.rtf);; PNG files (*.png)"
    );

    if(!filename.isNull() )
        return filename.toUtf8();

    return "";
}

void MainWindow::appendCode(QString code)
{
    ui->textBrowser_code->append(code);
}

void MainWindow::printResult(QString result)
{
    ui->textBrowser_result->append(result);
}
