#include "interpreter.h"
#include <QDebug>

Interpreter::Interpreter(map<int, StmtPtr>& stmts)
{
    statements = stmts;
    environment = Environment();
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

            if (pc != stmtIt->first) {
                auto tmpIt = statements.find(pc);
                if (tmpIt == statements.cend()) {
                    pc = stmtIt -> first;
                    throw RunTimeError("Target line doesn't exist.");
                } else
                    stmtIt = tmpIt;
            }
            else
                stmtIt++;
        }
    } catch (Error e) {
        qDebug() << "[Line " + QString::number(pc) + "]: "
                    + e.message;
    }
}
