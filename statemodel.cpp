#include "statemodel.h"
#include "boardmodel.h"

StateModel::StateModel(const BoardState& state, bool whitePlayerMovable, bool blackPlayerMovable)
    : mState(state)
    , mBlackMovable(blackPlayerMovable)
    , mWhiteMovable(whitePlayerMovable)
{
}

int StateModel::rowCount(const QModelIndex& /*parent*/) const
{
    return BoardModel::BOARD_HEIGHT * BoardModel::BOARD_WIDTH;
}


QVariant StateModel::data(const QModelIndex &index, int raw_role) const
{
    if (!index.isValid())
        return QVariant();

    const auto& board = BoardModel::Inst();
    const auto pos = QPoint(index.row() % BoardModel::BOARD_WIDTH, index.row() / BoardModel::BOARD_WIDTH);

    const auto role = StateRole(raw_role);

    if(role == StateRole::Position)
        return pos;

    bool isField = board.isField(pos);
    if(role == StateRole::IsOccupied)
        return isField && mState.stoneAt(pos) != Stone::None;


    if(role == StateRole::Stone)
    {
        if(!isField)
            return "red";
        Stone stone = mState.stoneAt(pos);
        switch(stone)
        {
        case Stone::Black:
            return "black";
        case Stone::White:
            return "white";
        default:
            return "red";
            break;
        }
    }

    return QVariant();
}

QHash<int, QByteArray> StateModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[(int)StateRole::Position] = "position";
    roles[(int)StateRole::IsOccupied] = "isOccupied";
    roles[(int)StateRole::Stone] = "stone";
    return roles;
}

bool StateModel::isValidMove(const QPoint &from, const QPoint &to) const
{
    const auto& board = BoardModel::Inst();
    return board.isField(to)
            && board.areAdjacentFields(from, to)
            && mState.stoneAt(to) == Stone::None;
}

bool StateModel::isValidMoveEnd(const QPoint &to) const
{
    return isValidMove(getMoveStartPosition(), to);
}

QPoint StateModel::getMoveStartPosition() const
{
    Q_ASSERT(!mMoveStart.isNull());
    return *mMoveStart.data();
}

const QString& StateModel::currentPlayer() const
{
    return to_string(mState.turn());
}

const QString &StateModel::phase() const
{
    return to_string(mState.phase());
}

bool StateModel::isMoving() const
{
    return mMoveStart.data() != nullptr;
}

bool StateModel::isMovablePlayer(const QString &name) const
{
    if(name == to_string(Stone::White)) {
        return mWhiteMovable;
    } else if(name == to_string(Stone::Black)) {
        return mBlackMovable;
    }
    return false;
}

void StateModel::startMove(const QPoint &from)
{
    Q_ASSERT(mState.stoneAt(from) == mState.turn());
    mMoveStart.reset(new QPoint(from));
    emit isMovingChanged();
}

void StateModel::abortMove()
{
    mMoveStart.reset();
    emit isMovingChanged();
}

void StateModel::endMove(const QPoint &to)
{
    Q_ASSERT(!mMoveStart.isNull());
    QPoint from = *mMoveStart.data();
    mMoveStart.reset();
    emit isMovingChanged();

    if(!isValidMove(from, to)) {
        return;
    }

    const auto beforeTurn = mState.turn();
    Move move(from, to);


    int from_row =  from.x() + from.y() * BoardModel::BOARD_WIDTH;
    int to_row =    to.x() + to.y() * BoardModel::BOARD_WIDTH;

    bool isAdjacent = std::abs(from_row - to_row) == 1;

    QModelIndex root;
    if(!isAdjacent)
    {
        beginRemoveRows(root, to_row, to_row);
        endRemoveRows();
        if(to_row < from_row)
        {
            --from_row;
        }
    }

    beginMoveRows(root, from_row, from_row, root, to_row + ((from_row + 1 == to_row)? 1 : 0));
    mState.move(move);
    endMoveRows();

    if(!isAdjacent)
    {
        beginInsertRows(root, from_row, from_row);
        endInsertRows();
    }

    if(beforeTurn != mState.turn())
    {
        emit currentPlayerChanged();
    }
}

void StateModel::put(const QPoint &to)
{
    const auto beforeTurn = mState.turn();

    mState.put(to);

    if(beforeTurn != mState.turn())
    {
        emit currentPlayerChanged();
    }
}

void StateModel::remove(const QPoint &to)
{
    const auto beforeTurn = mState.turn();

    Move move(to);
    mState.move(move);

    if(beforeTurn != mState.turn())
    {
        emit currentPlayerChanged();
    }
}

/*
bool StateModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (data(index, role) != value) {
        // FIXME: Implement me!
        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }
    return false;
}

Qt::ItemFlags StateModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable; // FIXME: Implement me!
}*/
