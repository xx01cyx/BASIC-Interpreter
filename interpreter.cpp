#include "interpreter.h"

Interpreter::Interpreter(map<int, ExprPtr>& exprs)
{
    expressions = exprs;
    parserTester = map<int, int>();
}

void Interpreter::interpret()
{
    map<int, ExprPtr>::const_iterator exprIt = expressions.cbegin();

    while (exprIt != expressions.cend()) {
        int currentLine = exprIt->first;
        ExprPtr expr = exprIt->second;
        parserTester[currentLine] = expr->evaluate();
        exprIt++;
    }
}
