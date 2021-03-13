#ifndef ERROR_H
#define ERROR_H

#include <QString>

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

#endif // ERROR_H
