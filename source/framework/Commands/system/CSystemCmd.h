#ifndef CCOMPILATIONCMD_H
#define CCOMPILATIONCMD_H

#include "framework/Commands/ITerminalCommand.h"

class CSystemCmd : public ITerminalCommand
{
public:
    CSystemCmd(QStringList args = QStringList());

protected:
    void run();
};

#endif // CCOMPILATIONCMD_H
