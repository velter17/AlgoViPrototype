#ifndef CCOMPILER_H
#define CCOMPILER_H

#include "framework/Commands/ITerminalCommand.h"
#include "framework/Commands/system/CSystemCmd.h"

class CCompiler : public ITerminalCommand
{
public:
    CCompiler(QStringList args = QStringList());
    QString getManualMessage();

protected:
    void run();
};

#endif // CCOMPILER_H
