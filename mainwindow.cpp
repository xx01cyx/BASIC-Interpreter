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
    filter = new Filter();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_load_clicked()
{
    QString filename = openFile();
    if (!filename.isEmpty())
        readFile(filename);

    auto lineIt = filter->lines.cbegin();

    while (lineIt != filter->lines.cend()) {
        ui->textBrowser_code->append(lineIt->second);
        lineIt++;
    }

}


void MainWindow::readFile(QString filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);

    try {
        QString line = in.readLine().trimmed();
        while (!line.isNull()) {
            filter->filterCode(line);
            line = in.readLine();
        }
    } catch (SyntaxError e) {
        qDebug() << e.message;
    }

    //scanner->setCode(code);

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

    ui->lineEdit_command->clear();
}

void MainWindow::on_pushButton_run_clicked()
{
    try {
        Scanner* scanner = new Scanner(filter->lines);
        scanner->scan();

        Parser* parser = new Parser(scanner->tokens);
        parser->parse();

        Interpreter* interpreter = new Interpreter(parser->statements);
        interpreter->interpret();

        for (auto line : scanner->tokens) {
            ui->textBrowser_result->append(QString::number(line.first));
            auto lineTokens = line.second;
            for (auto token : *(lineTokens))
                ui->textBrowser_result->append(token->toString());
        }

    } catch (Error e) {
        qDebug() << e.message << Qt::endl;
        exit(-1);
    }


//    for (auto line : interpreter->parserTester) {
//        ui->textBrowser_result->append(QString::number(line.first));
//        int parseResult = line.second;
//        ui->textBrowser_result->append(QString::number(parseResult));
//    }
}

