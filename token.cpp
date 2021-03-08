#include "token.h"

QString Token::TokenTypeString[] = {

    "REM", "LET", "PRINT", "INPUT", "GOTO", "IF", "THEN", "END",

    "NUMBER", "IDENTIFIER", "REM_CONTENT",

    "LEFT_PAREN", "RIGHT_PAREN", "PLUS",
    "MINUS", "MULTIPLY", "DEVIDE", "POWER",

    "EQUAL", "LESS", "GREATER",

};

ostream& operator << (ostream& os, Token& token)
{
    os << token.type << " " << token.lexeme.toStdString() << endl;
    return os;
}

QString Token::toString() const
{
    return TokenTypeString[type] + " " + lexeme;
}
