#include "parser.h"
#include <QDebug>

Parser::Parser()
{
    this->tokens = map<int, shared_ptr<Tokens>>();
    lineIt = this->tokens.cbegin();
    tokenIt = lineIt->second->cbegin();
    statements = map<int, StmtPtr>();
    errorSign = "";
    window = MainWindow::getInstance();
}

void Parser::setLineTokens(map<int, shared_ptr<Tokens>> &tokens)
{
    this->statements.clear();
    this->tokens = tokens;
    lineIt = this->tokens.cbegin();
    tokenIt = lineIt->second->cbegin();
}

void Parser::parse(bool requireAST)
{
    while (lineIt != tokens.cend()) {
        errorSign = "";
        parseLine(requireAST);
        lineIt++;
    }
}

void Parser::parseLine(bool requireAST)
{
    int currentLine = lineIt->first;
    tokenIt = lineIt->second->cbegin();

    statements[currentLine] = statement();

    if (errorSign.length() == 0 && tokenIt != lineIt->second->cend())
        statements[currentLine] = make_shared<ErrorStmt>("Invalid statement.");
    else if (errorSign.length() != 0)
        statements[currentLine] = make_shared<ErrorStmt>(errorSign);

    if (requireAST)
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
        case END: stmtType = "END"; break;
        case ERROR: stmtType = "ERROR";
    }

    QString ASTLine = QString::number(lineNumber) + " " + stmtType;
    window->printAST(0, ASTLine);
    stmtIt->second->generateAST();
}


StmtPtr Parser::statement()
{
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
    if (match(ERROR))
        return error();

    errorSign = "Invalid statement.";
    return make_shared<ErrorStmt>(errorSign);

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
        errorSign = "Expect variable name as a left value.";
        return make_shared<ErrorStmt>(errorSign);
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
        errorSign = "Expect variable name after INPUT.";
        return make_shared<ErrorStmt>(errorSign);
    }

    TokenPtr variable = previous();

    return make_shared<InputStmt>(variable);

}

StmtPtr Parser::goTo()
{
    if (!match(NUMBER)) {
        errorSign = "Expect line number after GOTO.";
        return make_shared<ErrorStmt>(errorSign);
    }

    TokenPtr line = previous();

    return make_shared<GotoStmt>(line);

}

StmtPtr Parser::ifThen()
{
    ExprPtr left = expression();

    if (!(match(LESS) || match(EQUAL) || match(GREATER))) {
        errorSign = "Expect comparison after IF.";
        return make_shared<ErrorStmt>(errorSign);
    }

    TokenType comparison = previous()->type;
    ExprPtr right = expression();

    if (!match(THEN)) {
        errorSign = "Expect THEN in IF statement.";
        return make_shared<ErrorStmt>(errorSign);
    }

    if (!match(NUMBER)) {
        errorSign = "Expect line number after THEN.";
        return make_shared<ErrorStmt>(errorSign);
    }

    TokenPtr line = previous();

    return make_shared<IfStmt>(left, comparison, right, line);

}

StmtPtr Parser::end()
{
    return make_shared<EndStmt>();
}

StmtPtr Parser::error()
{
    errorSign = previous()->lexeme;
    return make_shared<ErrorStmt>(errorSign);
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
        if (!match(RIGHT_PAREN)) {
            errorSign = "Expect ')'.";
            return make_shared<ErrorExpr>();
        }
        return expr;
    }

    errorSign = "Invalid expression.";
    return make_shared<ErrorExpr>();

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

