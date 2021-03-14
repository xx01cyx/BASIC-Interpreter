#include "filter.h"
#include <QDebug>

Filter::Filter()
{
    lines = map<int, QString>();
    window = MainWindow::getInstance();
    commands = {
        { "RUN", RUN },
        { "LOAD", LOAD },
        { "LIST", LIST },
        { "CLEAR", CLEAR },
        { "HELP", HELP },
        { "QUIT", QUIT },
    };
}

void Filter::filter(QString filename)
{
    if (!filename.isEmpty()){

        QFile file(filename);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return;

        QTextStream in(&file);

        try {
            QString line = in.readLine().trimmed();
            while (!line.isNull()) {
                this->filterLine(line);
                line = in.readLine();
            }
        } catch (SyntaxError e) {
            qDebug() << e.message;
        }
    }
}

void Filter::filterLine(QString line)
{
    int lineNumber = getLineNumber(line, true);

    if (lines.count(lineNumber))
        throw SyntaxError("Duplicate line number.");

    lines[lineNumber] = line;
}

int Filter::getLineNumber(QString line, bool isProgram)
{
    int current = 0, n = line.length();
    char c = line[current].unicode();

    if (!isdigit(c))
        if (isProgram)
            throw SyntaxError("Line number is missing.");
        else
            return -1;

    while (current != n && c != ' ') {
        current++;
        c = line[current].unicode();
    }

    bool ok;
    int lineNumber = line.mid(0, current).toInt(&ok, 10);

    if (!ok || lineNumber <= 0)
        throw SyntaxError("Invalid line number.");

    return lineNumber;
}

void Filter::filterCmd(QString cmd)
{
    int lineNumber = getLineNumber(cmd, false);

    // Basic program

    if (lineNumber > 0) {
        lines[lineNumber] = cmd;
        displayCode();
        return;
    }

    // Control commands

    try {
        if (!commands.count(cmd))
            throw CommandError("Invalid command.");

        Command command = commands[cmd];
        executeCommand(command);

    } catch (Error e) {
        qDebug() << e.message;
    }
}

void Filter::executeCommand(Command command)
{
    switch (command) {
        case RUN: emit run(); break;
        case LOAD: emit load(); break;
        case CLEAR: emit clear(); break;
        case HELP: emit help(); break;
        case QUIT: emit quit(); break;
    }
}

void Filter::displayCode()
{
    window->clearText();

    auto lineIt = lines.cbegin();

    while (lineIt != lines.cend()) {
        window->appendCode(lineIt->second);
        lineIt++;
    }
}


