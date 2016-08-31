#ifndef MINMAXAI_H
#define MINMAXAI_H

#include <queue>
#include <functional>
#include "ai.h"

class BoardState;

/*
 * An Ai that uses a alpha-beta-miniMax-algorithm to compute the next move.
 * It uses a simple heuristic to rate a specific state.
 */
class MinMaxAi: public Ai
{
public:
    Move nextMove(const BoardState &state) override;
    Put nextPut(const BoardState &state) override;

private:
    /*
     * Rates a state with the minimax-algo.
     * Lower is better.
     */
    double miniMax(int deep, const BoardState& state, Stone turn, double alpha, double beta) const;

    /*
     * Generate all possible successor states for a given state
     */
    void generateNexStates(const BoardState&, const std::function<bool(const BoardState&)>& callback) const;

    /*
     * Rates a state. Higher is better
     */
    double rateState(const BoardState& state) const;
};

#endif // MINMAXAI_H
