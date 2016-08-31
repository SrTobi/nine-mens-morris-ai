#ifndef AI_H
#define AI_H

#include <utility>
#include "move.h"

class BoardState;

/*
 * Interface for AIs. They need to calculate a next move or a next put
 */
class Ai
{
public:
    virtual ~Ai() = default;

    virtual Move nextMove(const BoardState& state) = 0;
    virtual Put nextPut(const BoardState& state) = 0;
};

#endif // AI_H
