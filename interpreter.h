#ifndef IMPLEMENTER_H
#define IMPLEMENTER_H

#include <memory>
#include <QString>
#include "expr.h"
#include "stmt.h"

using namespace std;

class Interpreter
{
private:

    map<int, StmtPtr> statements;
    int pc;

public:

    Environment environment;

    map<int, int> parserTester;

    Interpreter(map<int, StmtPtr>& stmts);

    void interpret();

};

#endif // IMPLEMENTER_H
