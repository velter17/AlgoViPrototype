#pragma once

enum class TSystemMode
{
    Interaction, // interact with graphic scene
    Draw,        // add new items
    Execution,   // waiting for algo execution
    Total        // wrong mode, just amount of modes
                 // add new type before it
};
