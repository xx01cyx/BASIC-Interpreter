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

void MainWindow::on_pushButton_save_clicked()
{
    saveFile();
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
          "Open File",
          QDir::currentPath(),
          "All files (*.*) ;; "
          "Document files (*.doc *.rtf);; "
          "PNG files (*.png)"
    );

    if(!filename.isNull() )
        return filename.toUtf8();

    return "";
}

void MainWindow::saveFile()
{
    QString code = ui->textBrowser_code->toPlainText();
    if (code.isEmpty()) {
        QMessageBox::warning(this, "Prompt", "Nothing to save.", QMessageBox::Yes);
        return;
    }

    QString fileName = QFileDialog::getSaveFileName(
        this,
        "Save File",
        QDir::currentPath(),
        "BASIC Files (*.basic);;"
        "Text Files (*.txt);;"
         "All Files(*.*)"
    );

    QFile file(fileName);

    if (file.open(QIODevice::WriteOnly|QIODevice::Text)) {
        QTextStream stream(&file);
        stream << ui->textBrowser_code->toPlainText();
        stream.flush();
        file.close();
        QMessageBox::information(this, "Success", "File saved.", QMessageBox::Yes);
    }
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

void MainWindow::printAST(int indent, QString content)
{
    QString AST = "";

    for (int i = 0; i < indent; ++i)
        AST += "    ";
    AST += content;

    ui->textBrowser_tree->append(AST);
}

