#include "boardstate.h"

BoardState::BoardState()
    : mTurn(Stone::None)
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
    return std::count(mFields.begin(), mFields.end(), Stone::White);
}

int BoardState::blackStones() const
{
    return std::count(mFields.begin(), mFields.end(), Stone::Black);
}

Stone BoardState::turn() const
{
    return mTurn;
}
