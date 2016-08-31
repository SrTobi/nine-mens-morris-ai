#ifndef RANDOMAI_H
#define RANDOMAI_H

#include "ai.h"
#include "move.h"

class BoardState;

/*
 * A simple Ai that chooses a valid random action.
 */
class RandomAi: public Ai
{
public:
    Move nextMove(const BoardState& state);
    Put nextPut(const BoardState &state);
};
#endif // RANDOMAI_H
