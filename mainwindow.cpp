#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QApplication>
#include <QFileDialog>
#include <QDebug>
#include "error.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scanner = new Scanner();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::displayCode()
{
    ui->textBrowser_code->setText(scanner->getCode());
}

void MainWindow::on_pushButton_load_clicked()
{
    QString filename = openFile();
    if (!filename.isEmpty())
        readFile(filename);
}


void MainWindow::readFile(QString filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    QString code = in.readAll();
    ui->textBrowser_code->setText(code);
    scanner->setCode(code);
    this->displayCode();
}

QString MainWindow::openFile()
{
    QString filename =  QFileDialog::getOpenFileName(
          this,
          "Open Document",
          QDir::currentPath(),
          "All files (*.*) ;; Document files (*.doc *.rtf);; PNG files (*.png)");

    if( !filename.isNull() )
    {
        return filename.toUtf8();
    }

    return "";
}

void MainWindow::on_lineEdit_command_returnPressed()
{
    QString code = ui->lineEdit_command->text();

    scanner->insertCode(code);

    this->displayCode();

    ui->lineEdit_command->clear();
}

void MainWindow::on_pushButton_run_clicked()
{
    try {
        scanner->scan();

        Parser* parser = new Parser(scanner->tokens);
        parser->parse();

        Interpreter* interpreter = new Interpreter(parser->statements);
        interpreter->interpret();

    } catch (Error e) {
        qDebug() << e.message << Qt::endl;
        exit(-1);
    }

    for (auto line : scanner->tokens) {
        ui->textBrowser_result->append(QString::number(line.first));
        auto lineTokens = line.second;
        for (auto token : *(lineTokens))
            ui->textBrowser_result->append(token->toString());
    }


//    for (auto line : interpreter->parserTester) {
//        ui->textBrowser_result->append(QString::number(line.first));
//        int parseResult = line.second;
//        ui->textBrowser_result->append(QString::number(parseResult));
//    }
}

