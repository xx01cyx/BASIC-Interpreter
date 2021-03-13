#include "filter.h"
#include <QDebug>

Filter::Filter()
{
    lines = map<int, QString>();
}

void Filter::filterCode(QString line)
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
