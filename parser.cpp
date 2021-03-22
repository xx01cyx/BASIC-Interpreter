#include "parser.h"
#include <QDebug>

Parser::Parser(map<int, shared_ptr<Tokens>> &tokens)
{
    this->tokens = tokens;
    lineIt = this->tokens.cbegin();
    tokenIt = lineIt->second->cbegin();
    statements = map<int, StmtPtr>();
    window = MainWindow::getInstance();
}

void Parser::parse()
{
    try {
        while (lineIt != tokens.cend()) {
            parseLine();
            lineIt++;
        }
    } catch (Error e) {
        qDebug() << "[Line " + QString::number(lineIt->first)
                    + "]: " + e.message;
        window->setProgramStatus(0);
        window->clearCmdPrompt();
    }
}

void Parser::parseLine()
{
    int currentLine = lineIt->first;
    tokenIt = lineIt->second->cbegin();

    statements[currentLine] = statement();

    printLineAST(currentLine);
}

void Parser::printLineAST(int lineNumber)
{
    auto stmtIt = statements.find(lineNumber);

    QString stmtType;

    switch (stmtIt->second->type) {
        case REM: stmtType = "REM"; break;
        case LET: stmtType = "LET ="; break;
        case PRINT: stmtType = "PRINT"; break;
        case INPUT: stmtType = "INPUT"; break;
        case GOTO: stmtType = "GOTO"; break;
        case IF: stmtType = "IF THEN"; break;
        case END: stmtType = "END";
    }

    QString ASTLine = QString::number(lineNumber) + " " + stmtType;

    window->printAST(0, ASTLine);
    stmtIt->second->generateAST();
}


StmtPtr Parser::statement()
{
    int currentLine = lineIt->first;

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

    throw ParseError("Invalid statement.");

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
    if (!match(IDENTIFIER))
        throw ParseError("Expect a variable name as a left value.");

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
    if (!match(IDENTIFIER))
        throw ParseError("Expect a variable name after INPUT.");

    TokenPtr variable = previous();

    return make_shared<InputStmt>(variable);

}

StmtPtr Parser::goTo()
{
    if (!match(NUMBER))
        throw ParseError("Expect a line number after GOTO.");

    TokenPtr line = previous();

    return make_shared<GotoStmt>(line);

}

StmtPtr Parser::ifThen()
{
    ExprPtr left = expression();

    if (!(match(LESS) || match(EQUAL) || match(GREATER)))
        throw ParseError("Expect comparison after IF.");

    TokenType comparison = previous()->type;
    ExprPtr right = expression();

    if (!match(THEN))
        throw ParseError("Expect THEN in IF statement.");

    if (!match(NUMBER))
        throw ParseError("Expect a line number after THEN.");

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
    int currentLine = lineIt->first;

    if (match(NUMBER))
        return make_shared<ConstantExpr>(previous()->lexeme.toInt());
    if (match(IDENTIFIER))
        return make_shared<IdentifierExpr>(previous()->lexeme);
    if (match(LEFT_PAREN)) {
        ExprPtr expr = expression();
        if (!match(RIGHT_PAREN))
            throw ParseError("Expect ')'.");
        return expr;
    }

    throw ParseError("Unexpected token.");
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
    return tokenIt == lineIt->second->cend();
}


void Parser::advance()
{
    shared_ptr<Tokens> lineTokens = lineIt->second;

    if (!endOfLine())
        tokenIt++;

}

TokenPtr Parser::previous()
{
    auto copyIt = tokenIt;
    copyIt--;
    return *copyIt;
}

