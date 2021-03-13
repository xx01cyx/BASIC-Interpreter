#include "filter.h"
#include <QDebug>

Filter::Filter()
{
    lines = map<int, QString>();
    window = MainWindow::getInstance();
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
    int lineNumber = getLineNumber(line);

    if (lines.count(lineNumber))
        throw SyntaxError("Duplicate line number!");

    lines[lineNumber] = line;
}

int Filter::getLineNumber(QString line)
{
    int current = 0, n = line.length();
    char c = line[current].unicode();

    if (!isdigit(c))
        throw SyntaxError("No line number!");

    while (current != n && isdigit(c)) {
        current++;
        c = line[current].unicode();
    }

    bool ok;
    int lineNumber = line.mid(0, current).toInt(&ok, 10);

    if (!ok)
        throw SyntaxError("Invalid line number!");

    return lineNumber;
}

void Filter::displayCode()
{
    auto lineIt = lines.cbegin();

    while (lineIt != lines.cend()) {
        window->appendCode(lineIt->second);
        lineIt++;
    }
}


