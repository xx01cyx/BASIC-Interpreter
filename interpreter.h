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
    map<int, StmtPtr>::const_iterator stmtIt;
    MainWindow* window;

public:

    Environment environment;

    Interpreter(map<int, StmtPtr>& stmts);
    void interpret();

};

#endif // IMPLEMENTER_H
