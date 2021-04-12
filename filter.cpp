#include "filter.h"
#include <QDebug>

Filter::Filter()
{
    lines = map<int, QString>();
    window = MainWindow::getInstance();
    commands = {
        { "LET", LET_CMD },
        { "PRINT", PRINT_CMD },
        { "INPUT", INPUT_CMD },
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

        QString line = in.readLine().trimmed();
        while (!line.isNull()) {
            this->filterLine(line);
            line = in.readLine();
        }
    }
}

void Filter::filterLine(QString line)
{
    int lineNumber = getLineNumber(line, true);

    if (lineNumber > 0)
        lines[lineNumber] = line;
}


/*
 * If string is from command line and does not start with a number, return 0.
 * If the line number exists, return the positive line number.
 * If the line number exceeds 1000000 or is less than 0, return -1 on error.
 */
int Filter::getLineNumber(QString line, bool isProgram)
{
    int current = 0, n = line.length();
    char c = line[current].unicode();

    if (!isdigit(c) && c != '-') {
        if (isProgram) {
            window->warn("Line number is missing.");
            return -1;
        }
        else
            return 0;
    }

    while (current != n && c != ' ') {
        current++;
        c = line[current].unicode();
    }

    bool ok;
    int lineNumber = line.mid(0, current).toInt(&ok, 10);

    if (!ok) {
        window->warn("Invalid line number.");
        return -1;
    } else if (lineNumber <= 0 || lineNumber > 1000000) {
        window->warn("Invalid line number " + QString::number(lineNumber) + ".");
        return -1;
    }

    return lineNumber;
}

void Filter::filterCmd(QString cmd)
{
    int lineNumber = getLineNumber(cmd, false);

    // Basic program

    if (lineNumber > 0) {

        bool ok;
        cmd.trimmed().toInt(&ok, 10);
        if (ok) {
            lines.erase(lineNumber);
            displayCode();
            return;
        }

        lines[lineNumber] = cmd;
        displayCode();
        return;
    }

    // Control commands

    else if (lineNumber == 0) {

        Command cmdType = judgeCmd(cmd);

        if (cmdType == ERROR_CMD) {
            window->warn("Invalid command.");
            return;
        }

        executeCommand(cmdType, cmd);
    }

}

Command Filter::judgeCmd(QString cmd)
{
    cmd = cmd.trimmed();

    if (commands.count(cmd))
        return commands[cmd];
    else if (cmd.startsWith("LET"))
        return LET_CMD;
    else if (cmd.startsWith("INPUT"))
        return INPUT_CMD;
    else if (cmd.startsWith("PRINT"))
        return PRINT_CMD;
    else
        return ERROR_CMD;
}

void Filter::executeCommand(Command cmdType, QString cmd)
{
    switch (cmdType) {
        case PRINT_CMD: case LET_CMD: case INPUT_CMD:
            emit cmdProgram(cmd); break;
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

void Filter::reset()
{
    lines.clear();
}



