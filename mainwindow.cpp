#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QApplication>
#include <QFileDialog>
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

    ui->textBrowser_code->setText(code);

    ui->lineEdit_command->clear();
}
