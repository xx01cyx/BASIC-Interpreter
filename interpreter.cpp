#include "interpreter.h"
#include <QDebug>

Interpreter::Interpreter(map<int, StmtPtr>& stmts)
{
    statements = stmts;
    environment = Environment();
    parserTester = map<int, int>();
    pc = stmts.begin()->first;
}

void Interpreter::interpret()
{
    map<int, StmtPtr>::const_iterator stmtIt = statements.cbegin();

    try {
        while (stmtIt != statements.cend()) {

            pc = stmtIt->first;
            StmtPtr stmt = stmtIt->second;
            stmt->execute(environment, pc);

            if (pc != stmtIt->first)
                stmtIt = statements.find(pc);
            else
                stmtIt++;

        }
    } catch (Error e) {
        qDebug() << "Line " + QString::number(pc) + ": "
                    + e.message << Qt::endl;
        exit(-1);
    }
}
