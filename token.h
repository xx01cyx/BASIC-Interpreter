#ifndef TOKEN_H
#define TOKEN_H

#include <QString>
#include <QDebug>
#include <iostream>
#include <memory>

using namespace std;

enum TokenType {

    REM, LET, PRINT, INPUT, GOTO, IF, THEN, END,

    NUMBER, IDENTIFIER, REM_CONTENT,

    LEFT_PAREN, RIGHT_PAREN, PLUS,
    MINUS, MULTIPLY, DEVIDE, POWER,

    EQUAL, LESS, GREATER,

};

class Token
{
public:
    TokenType type;
    QString lexeme;

    Token(TokenType t, QString l) : type(t), lexeme(l) {}
    Token(TokenType t) : type(t) { lexeme = ""; }

    QString toString() const;

    friend ostream& operator << (ostream& os, Token& token);

    static QString TokenTypeString[];

};


typedef shared_ptr<Token> TokenPtr;
typedef vector<TokenPtr> Tokens;


#endif // TOKEN_H
