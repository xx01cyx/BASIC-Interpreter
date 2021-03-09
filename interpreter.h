#ifndef IMPLEMENTER_H
#define IMPLEMENTER_H

#include <memory>
#include "expr.h"

class Interpreter
{
private:

    map<int, ExprPtr> expressions;


public:

    map<int, int> parserTester;

    Interpreter(map<int, ExprPtr>& exprs);
    void interpret();

};

#endif // IMPLEMENTER_H
