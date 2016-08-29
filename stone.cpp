#include "stone.h"


const QString& to_string(Stone stone)
{
    static const QString black("black");
    static const QString white("white");
    static const QString none("none");

    switch(stone)
    {
    case Stone::Black:
        return black;
    case Stone::White:
        return white;
    default:
        return none;
    }
}

Stone opponent(Stone stone)
{
    switch(stone)
    {
    case Stone::Black:
        return Stone::White;
    case Stone::White:
        return Stone::Black;
    default:
        return Stone::None;
    }
}
