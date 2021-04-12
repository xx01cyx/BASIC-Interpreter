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
    Environment global;
    Environment local;

public:

    Interpreter(Environment& globalEnvironment);
    void setStatements(map<int, StmtPtr>& stmts);
    void interpret(bool isCmd);
    Environment getGlobal() const;

};

#endif // IMPLEMENTER_H
