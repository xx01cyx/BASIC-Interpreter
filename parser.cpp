#include "parser.h"
#include <QDebug>

Parser::Parser(map<int, shared_ptr<Tokens>> &tokens)
{
    this->tokens = tokens;
    mapIt = this->tokens.cbegin();
    tokenIt = mapIt->second->cbegin();
    expressions = map<int, ExprPtr>();
}

void Parser::parse()
{
    int currentLine;

    while (mapIt != tokens.cend()) {

        currentLine = mapIt->first;
        tokenIt = mapIt->second->cbegin();

        expressions[currentLine] = expression();

        mapIt++;
    }
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
    TokenPtr token = *tokenIt;
    int currentLine = mapIt->first;

    if (match(NUMBER))
        return make_shared<ConstantExpr>(token->lexeme.toInt());
    if (match(IDENTIFIER))
        return make_shared<IdentifierExpr>(token->lexeme);
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
