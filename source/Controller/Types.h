/**
 * Project   Graviz
 *
 * @file     Types.h
 * @author   Dmytro Sadovyi
 * @date     03.12.2016
 */

#pragma once

namespace NController
{

enum class TTerminalState
{
    CommandInput,   // (default state) interaction with system
    InsideApp,      // interaction inside application
    Locked,         // interaction with terminal is disabled
    Question,       // waiting for answer of a question

};

enum class TTerminalCommandType
{
    System,         // system command to execute
    Application,    // application input
    HintRequest
};

enum class TCommandHintName
{
    SystemPath,
    AppName,
    Compile,
    RunSolver,
    RunSolverVisual,
    Test,
    Tester,

//    Total,
};


} // namespace NController
