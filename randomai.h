#ifndef RANDOMAI_H
#define RANDOMAI_H

#include "ai.h"
#include "move.h"

class BoardState;

class RandomAi: public Ai
{
public:
    Move nextMove(const BoardState& state);

    Put nextPut(const BoardState &state);
};
#endif // RANDOMAI_H
