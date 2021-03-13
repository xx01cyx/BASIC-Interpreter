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

void MainWindow::on_pushButton_clear_clicked()
{
    clearText();
}

void MainWindow::on_lineEdit_command_returnPressed()
{
    QString cmd = ui->lineEdit_command->text();
    ui->lineEdit_command->clear();
    emit(command(cmd));
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

bool MainWindow::checkRun()
{
    ui->textBrowser_result->clear();
    ui->textBrowser_tree->clear();

    QString code = ui->textBrowser_code->toPlainText();
    bool runable = !code.isEmpty();

    if (!runable)
        QMessageBox::information(this,tr("Error"),QStringLiteral("Please load a Basic file before running."),QMessageBox::Ok);

    return runable;
}

void MainWindow::printResult(QString result)
{
    ui->textBrowser_result->append(result);
}

void MainWindow::clearText()
{
    ui->textBrowser_code->clear();
    ui->textBrowser_result->clear();
    ui->textBrowser_tree->clear();
}

void MainWindow::helpInfo()
{
    QMessageBox::information(this, tr("Help"), QStringLiteral("Help Info"), QMessageBox::Ok);
}

