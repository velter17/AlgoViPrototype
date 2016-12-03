/**
 * Project   Graviz
 *
 * @file     Types.h
 * @author   Dmytro Sadovyi
 * @date     03.11.2016
 */

#pragma once

namespace NGraviz
{

enum class TSystemMode
{
    Interaction, // interact with graphic scene
    Draw,        // add new items
    Execution,   // waiting for algo execution
    Total        // wrong mode, just amount of modes
                 // add new type before it
};

enum class TProblemSolverType
{
    RealTime, // update answer each time, when we change input
    Straight, // update answer by query
    Interactive, // dialog with user
};


} // namespace NGraviz
