#include "interpreter.h"
#include <QDebug>

Interpreter::Interpreter(Environment& global)
{
    statements = map<int, StmtPtr>();
    stmtIt = statements.cbegin();
    window = MainWindow::getInstance();
    this->global = global;
    local = Environment();
}

void Interpreter::setStatements(map<int, StmtPtr> &stmts)
{
    statements = stmts;
    stmtIt = statements.cbegin();
}

void Interpreter::interpret(bool isCmd)
{
    int pc;

    try {
        while (stmtIt != statements.cend()) {
            pc = stmtIt->first;
            StmtPtr stmt = stmtIt->second;
            stmt->execute(global, local, pc, isCmd);

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
        QString errorMessage;
        if (isCmd)
            errorMessage = "[Error]: " + e.message;
        else
            errorMessage = "[Line " + QString::number(pc) + " Error]: " + e.message;
        window->printResult(errorMessage);
        window->setProgramStatus(0);
        window->clearCmdPrompt();
    }
}

Environment Interpreter::getGlobal() const
{
    return this->global;
}

