#include "basic.h"

Basic::Basic()
{
    window = MainWindow::getInstance();
    filter = new Filter();

    connect(window, SIGNAL(load()), this, SLOT(loadProgram()));
    connect(window, SIGNAL(run()), this, SLOT(runProgram()));
    connect(window, SIGNAL(command(QString)), this, SLOT(enterCommand(QString)));
    connect(filter, SIGNAL(run()), this, SLOT(runProgram()));
    connect(filter, SIGNAL(load()), this, SLOT(loadProgram()));
    connect(filter, SIGNAL(clear()), this, SLOT(clearProgram()));
    connect(filter, SIGNAL(help()), this, SLOT(getHelp()));
    connect(filter, SIGNAL(quit()), this, SLOT(quitInterpreter()));
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
    filter->reset();

    QString filename = window->openFile();

    filter->filter(filename);

    filter->displayCode();
}

void Basic::runProgram()
{
    if (!window->checkRun())
        return;

    window->setProgramStatus(1);

    Scanner* scanner = new Scanner(filter->lines);
    scanner->scan();

    Parser* parser = new Parser(scanner->tokens);
    parser->parse();

    Interpreter* interpreter = new Interpreter(parser->statements);
    interpreter->interpret();

    window->setProgramStatus(0);
    window->clearCmdPrompt();

}

void Basic::clearProgram()
{
    window->clearText();
    filter->reset();
}

void Basic::enterCommand(QString cmd)
{
    filter->filterCmd(cmd);
}

void Basic::getHelp()
{
    window->helpInfo();
}

void Basic::quitInterpreter()
{
    exit(0);
}


