#include "boardstate.h"
#include "minmaxai.h"


Move MinMaxAi::nextMove(const BoardState &state)
{
    double worstRate = std::numeric_limits<double>::infinity();
    Move result;
    state.generateMoves([&](const Move& move)
    {
        auto clone = state;
        clone.move(move);

        double rate = miniMax(3, clone, state.opponent(), -std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity());
        if(rate < worstRate)
        {
            worstRate = rate;
            result = move;
        }

        return true;
    });

    return result;
}

Put MinMaxAi::nextPut(const BoardState &state)
{
    double worstRate = std::numeric_limits<double>::infinity();
    Put result;
    state.generatePuts([&](const Put& put)
    {
        auto clone = state;
        clone.put(put);

        double rate = miniMax(2, clone, state.opponent(), -std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity());
        if(rate < worstRate)
        {
            worstRate = rate;
            result = put;
        }

        return true;
    });

    return result;
}

void MinMaxAi::generateNexStates(const BoardState& state, const std::function<bool (const BoardState &)>& callback) const
{
    Q_ASSERT(state.phase() == Phase::Move || state.phase() == Phase::Put);

    if(state.phase() == Phase::Move)
    {
        state.generateMoves([&state, &callback](const Move& move)
        {
            auto clone = state;
            clone.move(move);
            return callback(clone);
        });
    }else{
        state.generatePuts([&state, &callback](const Put& put)
        {
            auto clone = state;
            clone.put(put);
            return callback(clone);
        });
    }
}

double MinMaxAi::miniMax(int deep, const BoardState &state, Stone turn, double alpha, double beta) const
{
    const double winRate = 1000000.0;
    if(state.phase() == Phase::End)
        return state.turn() == turn? winRate : -winRate;
    if(deep <= 0)
        return rateState(state);

    double maxValue = alpha;

    generateNexStates(state, [&](const BoardState& newState)
    {
        double newStateValue = -miniMax(deep - 1, newState, opponent(turn), -beta, -maxValue);
        maxValue = std::max(newStateValue, maxValue);
        return maxValue < beta;
    });

    return maxValue;
}

double MinMaxAi::rateState(const BoardState &state) const
{
    double result = 0;

    auto turn = state.turn();
    auto opp = state.opponent();

    result += 10.0 * (state.stonesOf(turn) - state.stonesOf(opp));
    result += 1.0 * (state.possibleAdjacentMoves(turn) - state.possibleAdjacentMoves(opp));
    result += 40.0 * (state.numberOfMills(turn) - state.numberOfMills(opp));

    return result;
}
