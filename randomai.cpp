#include "randomai.h"
#include "boardstate.h"

Move RandomAi::nextMove(const BoardState &state)

{
    Move move;
    state.generateMoves([&](const Move& m)
    {
        move = m;
        return rand() % 3 > 0;
    });
    return move;
}

Put RandomAi::nextPut(const BoardState &state)
{
    Put put;
    state.generatePuts([&](const Put& p)
    {
        put = p;
        return rand() % 3 > 0;
    });

    return put;
}
