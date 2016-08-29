#include <exception>
#include "boardstate.h"

BoardState::BoardState()
    : mTurn(Stone::White)
    , mPhase(Phase::Move)
{
    mFields.fill(Stone::None);
}

Stone BoardState::stoneAt(int idx) const
{
    return mFields.at(idx);
}

Stone BoardState::stoneAt(const QPoint &pos) const
{
    auto& model = BoardModel::Inst();
    return stoneAt(model.positionToIndex(pos));
}

void BoardState::setStoneAt(int idx, Stone stone)
{
    mFields.at(idx) = stone;
}

void BoardState::setStoneAt(const QPoint &pos, Stone stone)
{
    auto& model = BoardModel::Inst();
    setStoneAt(model.positionToIndex(pos), stone);
}

int BoardState::whiteStones() const
{
    return stonesOf(Stone::White);
}

int BoardState::blackStones() const
{
    return stonesOf(Stone::Black);
}

int BoardState::stonesOf(Stone stone) const
{
    return std::count(mFields.begin(), mFields.end(), stone);
}

{
    return std::count(mFields.begin(), mFields.end(), Stone::Black);
}

Stone BoardState::turn() const
{
    return mTurn;
}

const QString &to_string(Phase phase)
{
    static const QString put("put");
    static const QString move("move");
    static const QString remove("remove");
    static const QString end("end");

    switch(phase)
    {
    case Phase::Put:
        return put;
    case Phase::Move:
        return move;
    case Phase::PutRemove:
    case Phase::Remove:
        return remove;
    case Phase::End:
        return end;
    default:
        throw std::logic_error("invalid phase!");
    }
}
