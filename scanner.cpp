#include "scanner.h"

Scanner::Scanner(map<int, QString>& lines)
{
    this->lines = lines;
    lineIt = this->lines.cbegin();
    tokens = map<int, shared_ptr<Tokens>>();
    current = 0;
    window = MainWindow::getInstance();

    keywords = {
        { "REM", REM },
        { "LET", LET },
        { "PRINT", PRINT },
        { "INPUT", INPUT },
        { "GOTO", GOTO },
        { "IF", IF },
        { "THEN", THEN },
        { "END", END },
    };
}


void Scanner::scan()
{   
    while (lineIt != lines.cend()) {
        scanLine();
        lineIt++;
    }
}

void Scanner::scanLine()
{
    int lineNumber = lineIt->first;
    QString code = lineIt->second;
    int n = code.length();        

    tokens[lineNumber] = make_shared<Tokens>(vector<TokenPtr>());

    current = QString::number(lineNumber).length();

    while (!isAtEnd())
        scanToken();

}


void Scanner::scanToken()
{
    char c = advance();

    switch (c) {
    case '(': addToken(LEFT_PAREN);  break;
    case ')': addToken(RIGHT_PAREN); break;
    case '+': addToken(PLUS); break;
    case '-': addToken(MINUS); break;
    case '/': addToken(DEVIDE); break;
    case '=': addToken(EQUAL); break;
    case '<': addToken(LESS); break;
    case '>': addToken(GREATER); break;
    case '*':
        if (peek() == '*') {
            addToken(POWER);
            advance();
        } else
            addToken(MULTIPLY);
        break;
    case ' ': case '\n': case '\t': case '\r': break;
    default:
        if (isdigit(c))
            scanNumber();
        else if (isalpha(c))
            scanIdentifier();
        else
            addToken(ERROR, "Unexpected character.");
    }
}

void Scanner::scanNumber()
{
    int start = current - 1;
    while (!isAtEnd() && isdigit(peek()))
        advance();

    QString lexeme = lineIt->second.mid(start, current - start);
    addToken(NUMBER, lexeme);
}

void Scanner::scanIdentifier()
{
    int start = current - 1;
    while (!isAtEnd() && (isalpha(peek()) || isdigit(peek())))
        advance();

    QString lexeme = lineIt->second.mid(start, current - start);

    if (keywords.count(lexeme)) {
        TokenType type = keywords[lexeme];
        addToken(type);

        if (type == REM)
            getRemark();
    } else
        addToken(IDENTIFIER, lexeme);
}

void Scanner::getRemark()
{
    int start = current;
    while (!isAtEnd() && peek() != '\n')
        advance();
    QString remark = lineIt->second.mid(start, current - start).trimmed();
    addToken(REM_CONTENT, remark);
}

void Scanner::addToken(TokenType type, QString lexeme)
{
    int lineNumber = lineIt->first;
    TokenPtr token = make_shared<Token>(type, lexeme);
    tokens[lineNumber]->push_back(token);
}

void Scanner::addToken(TokenType type)
{
    int lineNumber = lineIt->first;
    TokenPtr token = make_shared<Token>(type);
    tokens[lineNumber]->push_back(token);
}


char Scanner::advance()
{
    current++;
    return lineIt->second[current-1].unicode();
}

char Scanner::peek()
{
    return lineIt->second[current].unicode();
}

bool Scanner::isAtEnd()
{
    return current == lineIt->second.length();
}
