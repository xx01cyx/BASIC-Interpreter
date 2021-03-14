#ifndef UTILS_H
#define UTILS_H

#include <QString>

class EndProgram {};

class Error
{
public:
    QString message;

    Error(QString m) : message(m) {}
};

class SyntaxError : public Error
{
public:
    SyntaxError(QString m) : Error(m) {}
};

class ParseError : public Error
{
public:
    ParseError(QString m) : Error(m) {}
};

class RunTimeError : public Error
{
public:
    RunTimeError(QString m) : Error(m) {}
};

class CommandError : public Error
{
public:
    CommandError(QString m) : Error(m) {}
};

#endif // UTILS_H
