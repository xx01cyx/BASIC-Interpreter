#include "parser.h"
#include <QDebug>

Parser::Parser(map<int, shared_ptr<Tokens>> &tokens)
{
    this->tokens = tokens;
    mapIt = this->tokens.cbegin();
    tokenIt = mapIt->second->cbegin();
    statements = map<int, StmtPtr>();

}

void Parser::parse()
{
    while (mapIt != tokens.cend()) {

        parseLine();

        mapIt++;
    }
}

void Parser::parseLine()
{
    int currentLine = mapIt->first;
    tokenIt = mapIt->second->cbegin();

    statements[currentLine] = statement();

}


StmtPtr Parser::statement()
{
    int currentLine = mapIt->first;

    if (match(REM))
        return remark();
    if (match(LET))
        return let();
    if (match(PRINT))
        return print();
    if (match(INPUT))
        return input();
    if (match(GOTO))
        return goTo();
    if (match(IF))
        return ifThen();
    if (match(END))
        return end();

    throw ParseError("Invalid statement at line " + QString::number(currentLine) + ".");

}

StmtPtr Parser::remark()
{
    if (match(REM_CONTENT)) {
        return make_shared<RemarkStmt>(previous());
    }

    TokenPtr emptyRemark = make_shared<Token>(REM_CONTENT);
    return make_shared<RemarkStmt>(emptyRemark);

}

StmtPtr Parser::let()
{
    if (!match(IDENTIFIER)) {
        int currentLine = mapIt->first;
        throw ParseError("Expect a variable name as a left value at line "
                         + QString::number(currentLine) + ".");
    }

    TokenPtr variable = previous();
    ExprPtr initializer = nullptr;

    if (match(EQUAL))
        initializer = expression();

    return make_shared<LetStmt>(variable, initializer);

}

StmtPtr Parser::print()
{
    ExprPtr expr = expression();

    return make_shared<PrintStmt>(expr);

}

StmtPtr Parser::input()
{
    if (!match(IDENTIFIER)) {
        int currentLine = mapIt->first;
        throw ParseError("Expect a variable name after INPUT at line "
                         + QString::number(currentLine) + ".");
    }

    TokenPtr variable = previous();

    return make_shared<InputStmt>(variable);

}

StmtPtr Parser::goTo()
{
    if (!match(NUMBER)) {
        int currentLine = mapIt->first;
        throw ParseError("Expect a line number after GOTO at line "
                         + QString::number(currentLine) + ".");
    }

    TokenPtr line = previous();

    return make_shared<GotoStmt>(line);

}

StmtPtr Parser::ifThen()
{
    int currentLine = mapIt->first;

    ExprPtr left = expression();

    if (!(match(LESS) || match(EQUAL) || match(GREATER))) {
        throw ParseError("Expect comparison after IF at line "
                         + QString::number(currentLine) + ".");
    }

    TokenType comparison = previous()->type;
    ExprPtr right = expression();

    if (!match(THEN)) {
        throw ParseError("Expect THEN in IF statement at line "
                         + QString::number(currentLine) + ".");
    }

    if (!match(NUMBER)) {
        throw ParseError("Expect a line number after THEN at line "
                         + QString::number(currentLine) + ".");
    }

    TokenPtr line = previous();

    return make_shared<IfStmt>(left, comparison, right, line);

}

StmtPtr Parser::end()
{
    return make_shared<EndStmt>();
}



ExprPtr Parser::expression()
{
    return term();
}

ExprPtr Parser::term()
{
    ExprPtr expr = factor();

    while (!endOfLine() && (match(PLUS) || match(MINUS))) {
        TokenPtr op = previous();
        expr = make_shared<CompoundExpr>(expr, op->type, factor());
    }

    return expr;
}

ExprPtr Parser::factor()
{
    ExprPtr expr = unaryPower();

    while (!endOfLine() && (match(MULTIPLY) || match(DEVIDE))) {
        TokenPtr op = previous();
        expr = make_shared<CompoundExpr>(expr, op->type, unaryPower());
    }

    return expr;
}

ExprPtr Parser::unaryPower()
{
    if (match(MINUS))
        return make_shared<UnaryExpr>(unaryPower());
    return power();
}

ExprPtr Parser::power()
{
    ExprPtr expr = unary();
    if (match(POWER))
        expr = make_shared<CompoundExpr>(expr, POWER, power());
    return expr;
}

ExprPtr Parser::unary()
{
    if (match(MINUS)) {
        return make_shared<UnaryExpr>(unary());
    }
    return primary();
}


ExprPtr Parser::primary()
{
    int currentLine = mapIt->first;

    if (match(NUMBER))
        return make_shared<ConstantExpr>(previous()->lexeme.toInt());
    if (match(IDENTIFIER))
        return make_shared<IdentifierExpr>(previous()->lexeme);
    if (match(LEFT_PAREN)) {
        ExprPtr expr = expression();
        if (!match(RIGHT_PAREN))
            throw ParseError("Expect ')' at line " + QString::number(currentLine) + ".");
        return expr;
    }

    throw ParseError("Unexpected character at line " + QString::number(currentLine) + ".");
}


bool Parser::match(TokenType type)
{
    if (!endOfLine() && (*tokenIt)->type == type) {
        advance();
        return true;
    }
    return false;

}

bool Parser::endOfLine()
{
    return tokenIt == mapIt->second->cend();
}


void Parser::advance()
{
    shared_ptr<Tokens> lineTokens = mapIt->second;

    if (!endOfLine())
        tokenIt++;

}

TokenPtr Parser::previous()
{
    auto copyIt = tokenIt;
    copyIt--;
    return *copyIt;
}

