#ifndef AI_H
#define AI_H

#include <utility>
#include "move.h"

class BoardState;


class Ai
{
public:
    virtual ~Ai() = default;

    virtual Move nextMove(const BoardState& state) = 0;
    virtual Put nextPut(const BoardState& state) = 0;
};

#endif // AI_H
