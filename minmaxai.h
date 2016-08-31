#ifndef MINMAXAI_H
#define MINMAXAI_H

#include <queue>
#include <functional>
#include "ai.h"

class BoardState;

class MinMaxAi: public Ai
{
public:
    Move nextMove(const BoardState &state) override;
    Put nextPut(const BoardState &state) override;

private:
    double miniMax(int deep, const BoardState& state, Stone turn, double alpha, double beta) const;
    void generateNexStates(const BoardState&, const std::function<bool(const BoardState&)>& callback) const;
    double rateState(const BoardState& state) const;
};

#endif // MINMAXAI_H
