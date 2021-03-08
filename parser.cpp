#include "parser.h"

Parser::Parser(map<int, shared_ptr<Tokens>> &tokens)
{
    this->tokens = tokens;
    mapIt = tokens.begin();
    tokenIt = mapIt->second->begin();
    expressions = map<int, ExprPtr>();
}

void Parser::parse()
{
    int currentLine;

    while (mapIt != tokens.end()) {

        currentLine = mapIt->first;
        tokenIt = mapIt->second->begin();

        expressions[currentLine] = term();

        mapIt++;
    }

}

ExprPtr Parser::term()
{
    ExprPtr expr = factor();
    TokenPtr op = *tokenIt;
    if (match(PLUS) || match(MINUS))
        return make_shared<CompoundExpr>(expr, op->type, factor());
    return expr;
}

ExprPtr Parser::factor()
{
    ExprPtr expr = unaryPower();
    TokenPtr op = *tokenIt;
    if (match(MULTIPLY) || match(DEVIDE))
        return make_shared<CompoundExpr>(expr, op->type, unaryPower());
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
        return make_shared<CompoundExpr>(expr, POWER, power());
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
    if (match(NUMBER))
        return make_shared<ConstantExpr>(token->lexeme.toInt());
    if (match(IDENTIFIER))
        return make_shared<IdentifierExpr>(token->lexeme);
    throw ParseError("Unexpected character at line " + QString::number(mapIt->first) + ".");
}


bool Parser::match(TokenType type)
{
    if ((*tokenIt)->type == type) {
        advance();
        return true;
    }
    return false;

}

void Parser::advance()
{
    shared_ptr<Tokens> lineTokens = mapIt->second;

    if (tokenIt != lineTokens->end())
        tokenIt++;

}

TokenPtr Parser::previous()
{
    auto copyIt = tokenIt;
    copyIt--;
    return *copyIt;
}
