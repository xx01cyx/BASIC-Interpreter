#include "basic.h"

Basic::Basic()
{
    global = Environment();
    window = MainWindow::getInstance();
    filter = new Filter();
    scanner = new Scanner();
    parser = new Parser();

    connect(window, SIGNAL(load()), this, SLOT(loadProgram()));
    connect(window, SIGNAL(run()), this, SLOT(runProgram()));
    connect(window, SIGNAL(clear()), this, SLOT(clearProgram()));
    connect(window, SIGNAL(command(QString)), this, SLOT(enterCommand(QString)));
    connect(filter, SIGNAL(cmdProgram(QString)), this, SLOT(receiveCmdProgram(QString)));
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

void Basic::receiveCmdProgram(QString cmd)
{
    map<int, QString> cmdProgram = map<int, QString>();
    cmdProgram[0] = cmd;

    window->clearAST();
    window->clearResult();

    window->setProgramStatus(1);

    scanner->setLines(cmdProgram);
    scanner->scan();

    parser->setLineTokens(scanner->tokens);
    parser->parse(false);

    interpreter = new Interpreter(this->global);
    interpreter->setStatements(parser->statements);
    interpreter->interpret(true);

    this->global = interpreter->getGlobal();

    window->setProgramStatus(0);
    window->clearCmdPrompt();

}

void Basic::executeCmdProgram(StmtPtr stmt)
{
    Environment empty = Environment();
    int pc = 0;

    try {
        stmt->execute(global, empty, pc, true);

    } catch (Error e) {
        QString errorMessage = "[Error]: " + e.message;
        window->printResult(errorMessage);
        window->setProgramStatus(0);
        window->clearCmdPrompt();
    }
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

    scanner->setLines(filter->lines);
    scanner->scan();

    parser->setLineTokens(scanner->tokens);
    parser->parse(true);

    interpreter = new Interpreter(global);
    interpreter->setStatements(parser->statements);
    interpreter->interpret(false);

    window->setProgramStatus(0);
    window->clearCmdPrompt();

}

void Basic::clearProgram()
{
    global.clear();
    filter->reset();
    window->clearText();
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


