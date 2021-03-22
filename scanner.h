#ifndef SCANNER_H
#define SCANNER_H

#include <QDebug>
#include <QString>
#include <QChar>
#include <map>
#include <vector>
#include <unordered_map>
#include <memory>
#include "mainwindow.h"
#include "token.h"
#include "utils.h"

using namespace std;

class Scanner
{
public:

    map<int, shared_ptr<Tokens>> tokens;

    Scanner(map<int, QString>& lines);
    void scan();

private:

    map<int, QString> lines;
    map<int, QString>::const_iterator lineIt;
    int current;
    unordered_map<QString, TokenType> keywords;
    MainWindow* window;

    void scanLine();
    void scanToken();
    void scanNumber();
    void scanIdentifier();
    void getRemark();

    void addToken(TokenType type, QString lexeme);
    void addToken(TokenType type);

    char advance();
    char peek();
    bool isAtEnd();
};

#endif // SCANNER_H
