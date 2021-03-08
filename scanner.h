#ifndef SCANNER_H
#define SCANNER_H

#include <QDebug>
#include <QString>
#include <QChar>
#include <map>
#include <vector>
#include <unordered_map>
#include <memory>
#include "token.h"
#include "error.h"

using namespace std;

class Scanner
{
public:

    map<int, shared_ptr<Tokens>> tokens;

    Scanner();

    void setCode(QString code);
    void insertCode(QString newCode);
    QString getCode();

    void scan();

private:

    QString code;
    int current;
    unordered_map<QString, TokenType> keywords;

    void scanLine();
    int getLineNumber();
    void scanToken(int lineNumber);
    void scanNumber(int lineNumber);
    void scanIdentifier(int lineNumber);
    void getRemark(int lineNumber);

    void addToken(int lineNumber, TokenType type, QString lexeme);
    void addToken(int lineNumber, TokenType type);

    char advance();
    char peek();
};

#endif // SCANNER_H
