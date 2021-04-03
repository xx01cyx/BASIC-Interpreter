#include "interpreter.h"
#include <QDebug>

Interpreter::Interpreter(map<int, StmtPtr>& stmts)
{
    statements = stmts;
    stmtIt = statements.cbegin();
    window = MainWindow::getInstance();
    environment = Environment();
}

void Interpreter::interpret()
{
    int pc;

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
    } catch (EndProgram end) {

    } catch (Error e) {
        QString errorMessage = "[Line " + QString::number(pc) + " Error]: " + e.message;
        window->printResult(errorMessage);
        window->setProgramStatus(0);
        window->clearCmdPrompt();
    }
}

