#include "boardmodel.h"
#include "move.h"

Move::Move()
    : mFrom(-1)
    , mTo(-1)
    , mRemove(-1)
{
}

Move::Move(int from, int to)
    : Move(from, to, -1)
{
}

Move::Move(int remove)
    : Move(-1, -1, remove)
{
}

Move::Move(int from, int to, int remove)
    : mFrom(from)
    , mTo(to)
    , mRemove(remove)
{
    const auto& board = BoardModel::Inst();
    Q_ASSERT(from == -1 || board.isField(from));
    Q_ASSERT(to == -1 || board.isField(to));
    Q_ASSERT(remove == -1 || board.isField(remove));
    Q_ASSERT(isValid());
}

int ptoi(const QPoint& pos)
{
    const auto& board = BoardModel::Inst();
    return board.positionToIndex(pos);
}

Move::Move(const QPoint &from, const QPoint &to)
    : Move(ptoi(from), ptoi(to))
{
}

Move::Move(const QPoint &remove)
    : Move(ptoi(remove))
{
}

Move::Move(const QPoint &from, const QPoint &to, const QPoint &remove)
    : Move(ptoi(from), ptoi(to), ptoi(remove))
{
}

int Move::fromIdx() const
{
    Q_ASSERT(isMoving());
    return mFrom;
}

int Move::toIdx() const
{
    Q_ASSERT(isMoving());
    return mTo;
}

int Move::removeIdx() const
{
    Q_ASSERT(isRemoving());
    return mRemove;
}

QPoint Move::from() const
{
    const auto& board = BoardModel::Inst();
    return board.indexToPosition(fromIdx());
}

QPoint Move::to() const
{
    const auto& board = BoardModel::Inst();
    return board.indexToPosition(toIdx());
}

QPoint Move::remove() const
{
    const auto& board = BoardModel::Inst();
    return board.indexToPosition(removeIdx());
}

bool Move::isValid() const
{
    return isRemoving() || isMoving();
}

bool Move::isRemoving() const
{
    return mRemove != -1;
}

bool Move::isMoving() const
{
    return mFrom != -1 && mTo != -1;
}


Put::Put()
    : mTo(-1)
    , mRemove(-1)
{
}

Put::Put(int to)
    : Put(to, -1)
{
}

Put::Put(int to, int remove)
    : mTo(to)
    , mRemove(remove)
{
    const auto& board = BoardModel::Inst();
    Q_ASSERT(board.isField(to));
    Q_ASSERT(remove == -1 || board.isField(remove));
}

Put::Put(const QPoint &to)
    : Put(ptoi(to), -1)
{
}

Put::Put(const QPoint &to, const QPoint &remove)
    : Put(ptoi(to), ptoi(remove))
{
}

int Put::toIdx() const
{
    return mTo;
}

int Put::removeIdx() const
{
    Q_ASSERT(isRemoving());
    return mRemove;
}

QPoint Put::to() const
{
    const auto& board = BoardModel::Inst();
    return board.indexToPosition(toIdx());
}

QPoint Put::remove() const
{
    const auto& board = BoardModel::Inst();
    return board.indexToPosition(removeIdx());
}

bool Put::isValid() const
{
    return mTo >= 0;
}

bool Put::isRemoving() const
{
    return mRemove != -1;
}
