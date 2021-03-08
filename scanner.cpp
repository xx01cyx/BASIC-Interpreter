#include "scanner.h"

Scanner::Scanner()
{
    code = "";
    current = 0;
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

void Scanner::setCode(QString code)
{
    this->code = code;
}

void Scanner::insertCode(QString newCode)
{
    this->code += ("\n" + newCode);
}

QString Scanner::getCode()
{
    return this->code;
}

void Scanner::scan()
{
    int n = code.length();
    qDebug() << "code length: " << n << Qt::endl;
    while (current != n)
        scanLine();
}

void Scanner::scanLine()
{
    char c = peek();
    if (!isdigit(c))
        throw GrammarError("A line number at the beginning of each line is missing.");

    int lineNumber = getLineNumber();

    while (peek() != '\n' && peek() != '\0')
        scanToken(lineNumber);

    while (peek() == '\n')
        advance();   // Skip '\n'
    qDebug() << "current: " << current << Qt::endl;
}

int Scanner::getLineNumber()
{
    int start = current;
    while (isdigit(peek()))
        advance();

    QString lineNumberStr = code.mid(start, current - start);
    int lineNumber = lineNumberStr.toInt();
    qDebug() << lineNumber << Qt::endl;

    tokens[lineNumber] = make_shared<vector<shared_ptr<Token>>>();

    return lineNumber;
}

void Scanner::scanToken(int lineNumber)
{
    char c = advance();

    switch (c) {
    case '(': addToken(lineNumber, LEFT_PAREN);  break;
    case ')': addToken(lineNumber, RIGHT_PAREN); break;
    case '+': addToken(lineNumber, PLUS); break;
    case '-': addToken(lineNumber, MINUS); break;
    case '/': addToken(lineNumber, DEVIDE); break;
    case '=': addToken(lineNumber, EQUAL); break;
    case '<': addToken(lineNumber, LESS); break;
    case '>': addToken(lineNumber, GREATER); break;
    case '*':
        if (peek() == '*') {
            addToken(lineNumber, POWER);
            advance();
        } else
            addToken(lineNumber, MULTIPLY);
        break;
    case ' ': case '\n': case '\t': case '\r': break;
    default:
        if (isdigit(c))
            scanNumber(lineNumber);
        else if (isalpha(c))
            scanIdentifier(lineNumber);
    }
}

void Scanner::scanNumber(int lineNumber)
{
    int start = current - 1;
    while (isdigit(peek()))
        advance();
    QString lexeme = code.mid(start, current - start);
    addToken(lineNumber, NUMBER, lexeme);
}

void Scanner::scanIdentifier(int lineNumber)
{
    int start = current - 1;
    while (isalpha(peek()) || isdigit(peek()))
        advance();
    QString lexeme = code.mid(start, current - start);

    if (keywords.count(lexeme)) {
        TokenType type = keywords[lexeme];
        addToken(lineNumber, type);

        if (type == REM)
            getRemark(lineNumber);
    } else
        addToken(lineNumber, IDENTIFIER, lexeme);
}

void Scanner::getRemark(int lineNumber)
{
    int start = current;
    while (peek() != '\n')
        advance();
    QString remark = code.mid(start, current - start).trimmed();
    addToken(lineNumber, REM_CONTENT, remark);
}

void Scanner::addToken(int lineNumber, TokenType type, QString lexeme)
{
    TokenPtr token = make_shared<Token>(type, lexeme);
    tokens[lineNumber]->push_back(token);
}

void Scanner::addToken(int lineNumber, TokenType type)
{
    TokenPtr token = make_shared<Token>(type);
    tokens[lineNumber]->push_back(token);
}


char Scanner::advance()
{
    current++;
    return code[current-1].unicode();
}

char Scanner::peek()
{
    return code[current].unicode();
}
