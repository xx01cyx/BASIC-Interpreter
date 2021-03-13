#include "basic.h"

Basic::Basic()
{
    window = MainWindow::getInstance();
    filter = new Filter();

    connect(window, SIGNAL(load()), this, SLOT(loadProgram()));
    connect(window, SIGNAL(run()), this, SLOT(runProgram()));
}

Basic::~Basic()
{
    delete filter;
}

void Basic::run()
{
    window->show();
}

void Basic::loadProgram()
{
    QString filename = window->openFile();

    filter->filter(filename);

    filter->displayCode();
}

void Basic::runProgram()
{
    Scanner* scanner = new Scanner(filter->lines);
    scanner->scan();

    Parser* parser = new Parser(scanner->tokens);
    parser->parse();

    Interpreter* interpreter = new Interpreter(parser->statements);
    interpreter->interpret();
}

