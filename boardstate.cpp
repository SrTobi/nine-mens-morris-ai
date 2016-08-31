#include <exception>
#include "boardstate.h"

BoardState::BoardState()
    : mTurn(Stone::White)
    , mPhase(Phase::Put)
    , mPutStones(0)
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

bool BoardState::isValidMove(const Move& move) const
{
    const auto& board = BoardModel::Inst();

    if(!move.isValid())
        return false;

    if(move.isRemoving())
    {
        if(stoneAt(move.removeIdx()) != opponent())
            return false;

        if(millAt(move.removeIdx()) == opponent())
            return false;
    }

    if(move.isMoving())
    {
        if(phase() != Phase::Move)
            return false;

        if(stoneAt(move.fromIdx()) != turn())
            return false;

        if(stoneAt(move.toIdx()) != Stone::None)
            return false;

        if(stonesOf(turn()) > BoardModel::NUM_STONES_ALLOW_FLY && !board.areAdjacentFields(move.fromIdx(), move.toIdx()))
            return false;

        if(move.isRemoving())
        {
            if(millAt(move.to()) != turn())
                return false;
        }
    }
    else if(move.isRemoving())
    {
        // no move, only remove
        if(phase() != Phase::Remove && phase() != Phase::PutRemove)
            return false;
    }

    return true;
}

void BoardState::move(const Move& move)
{
    Q_ASSERT(isValidMove(move));

    bool nextTurn = false;

    if(move.isMoving())
    {
        setStoneAt(move.fromIdx(), Stone::None);
        setStoneAt(move.toIdx(), turn());
        if(millAt(move.to()) == turn())
        {
            mPhase = Phase::Remove;

            // check for winner
            if(stonesOf(opponent()) == BoardModel::NUM_STONES_ALLOW_FLY)
            {
                mPhase = Phase::End;
                return;
            }

            const auto& board = BoardModel::Inst();
            // check if a stone stoneof the opponent can be removed!
            auto& fields = board.fields();
            int numRemovableStones = std::count_if(fields.begin(), fields.end(),
                                                   [this](const QPoint& pos)
                                                    {
                                                        return stoneAt(pos) == opponent() && millAt(pos) == Stone::None;
                                                    });
            if(numRemovableStones <= 0)
            {
                mPhase = Phase::Move;
                nextTurn = true;
            }
        }else{
            nextTurn = true;
        }
    }

    if(move.isRemoving())
    {
        Q_ASSERT(phase() == Phase::Remove || phase() == Phase::PutRemove);
        setStoneAt(move.removeIdx(), Stone::None);
        if(phase() == Phase::PutRemove && mPutStones < BoardModel::NUM_STONES_TO_PUT*2)
            mPhase = Phase::Put;
        else
            mPhase = Phase::Move;
        nextTurn = true;
    }

    if(nextTurn)
        mTurn = opponent();
}

bool BoardState::isValidPut(const Put &put) const
{
    if(!put.isValid())
        return false;

    if(phase() != Phase::Put)
        return false;

    if(stoneAt(put.toIdx()) != Stone::None)
        return false;

    if(put.isRemoving())
    {
        if(stoneAt(put.removeIdx()) != opponent())
            return false;

        if(millAt(put.removeIdx()) == opponent())
            return false;
    }

    return true;
}

void BoardState::put(int idx)
{
    Put put(idx);
    this->put(put);
}

void BoardState::put(const QPoint &pos)
{
    auto& model = BoardModel::Inst();
    put(model.positionToIndex(pos));
}

void BoardState::put(const Put &put)
{
    int idx = put.toIdx();
    Q_ASSERT(isValidPut(put));

    ++mPutStones;
    setStoneAt(idx, turn());

    if(millAt(idx) == turn())
    {
        mPhase = Phase::PutRemove;
    } else {
        mTurn = opponent();
        if(mPutStones >= BoardModel::NUM_STONES_TO_PUT * 2)
            mPhase = Phase::Move;
    }
}

Stone BoardState::millAt(int idx) const
{
    auto& model = BoardModel::Inst();
    return millAt(model.indexToPosition(idx));
}

Stone BoardState::millAt(const QPoint& pos) const
{
    const auto& board = BoardModel::Inst();

    Stone stone = stoneAt(pos);
    if(stone != Stone::None)
    {
        for(const QPoint& neighbour : board.adjacentFields(pos))
        {
            if(stoneAt(neighbour) != stone)
                continue;

            auto diff = neighbour - pos;

            // look ahead
            auto ahead = neighbour + diff;
            if(board.isField(ahead) && stoneAt(ahead) == stone)
                return stone;

            // look back
            auto back = pos - diff;
            if(board.isField(back) && stoneAt(back) == stone)
                return stone;
        }
    }
    return Stone::None;
}

void BoardState::generateMoves(const std::function<bool(const Move&)> &callback) const
{
    const auto& board = BoardModel::Inst();

    for(int from = 0; from < BoardModel::BOARD_FIELDS_NUM; ++from)
    {
        if(stoneAt(from) == turn())
        {
            const auto& possibleFields = stonesOf(turn()) <= BoardModel::NUM_STONES_ALLOW_FLY?
                            board.fieldIndices() : board.adjacentFields(from);
            for(int to : possibleFields)
            {
                Move move(from, to);
                if(isValidMove(move))
                {
                    BoardState clone = *this;
                    clone.move(move);
                    if(clone.phase() == Phase::Remove)
                    {
                        for(int rfield = 0; rfield < BoardModel::BOARD_FIELDS_NUM; ++rfield)
                        {
                            Move moveRemove(from, to, rfield);
                            if(isValidMove(moveRemove))
                            {
                                if(!callback(moveRemove))
                                    return;
                            }
                        }
                    }else{
                        if(!callback(move))
                            return;
                    }
                }
            }
        }

    }
}

void BoardState::generatePuts(const std::function<bool(const Put&)> &callback) const
{
    Q_ASSERT(phase() == Phase::Put);

    for(int field = 0; field < BoardModel::BOARD_FIELDS_NUM; ++field)
    {
        Put p(field);
        if(isValidPut(p))
        {
            BoardState clone = *this;
            clone.put(p);
            if(clone.phase() == Phase::PutRemove)
            {
                for(int rfield = 0; rfield < BoardModel::BOARD_FIELDS_NUM; ++rfield)
                {
                    Put putRemove(field, rfield);
                    if(isValidPut(putRemove))
                    {
                        if(!callback(putRemove))
                            return;
                    }
                }
            }else{
                if(!callback(p))
                    return;
            }
        }
    }
}

Stone BoardState::turn() const
{
    return mTurn;
}

Stone BoardState::opponent() const
{
    return ::opponent(turn());
}

Phase BoardState::phase() const
{
    return mPhase;
}

bool BoardState::hasEnded() const
{
    return mPhase == Phase::End;
}

Stone BoardState::winner() const
{
    Q_ASSERT(hasEnded());
    return mTurn;
}

Stone BoardState::looser() const
{
    Q_ASSERT(hasEnded());
    return ::opponent(mTurn);
}

QString BoardState::toString() const
{
    auto& board = BoardModel::Inst();
    QString result;

    for(int y = 0; y < BoardModel::BOARD_HEIGHT; ++y)
    {
        for(int x = 0; x < BoardModel::BOARD_WIDTH; ++x)
        {
            if(board.isField(QPoint(x, y)))
                switch (stoneAt(QPoint(x, y))) {
                case Stone::Black:
                    result += 'B';
                    break;
                case Stone::White:
                    result += 'W';
                    break;
                default:
                    result += '+';
                    break;
                }
            else
                result += ' ';
        }
        result += '\n';
    }
    return result + "Phase: " + to_string(phase()) + "\n";
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
