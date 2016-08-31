#include <qdebug.h>
#include <QRunnable>
#include <QThreadPool>
#include "statemodel.h"
#include "aiworker.h"
#include "randomai.h"
#include "minmaxai.h"
#include "boardmodel.h"


StateModel::StateModel(const BoardState& state, const std::shared_ptr<Ai>& whiteAi, const std::shared_ptr<Ai>& blackAi)
    : mState(state)
    , mWhiteAi(whiteAi)
    , mBlackAi(blackAi)
{
    for(int i = 0; i < BoardModel::BOARD_WIDTH * BoardModel::BOARD_HEIGHT; ++i)
        mRowTable.push_back(i);

    /*
     * If the player changes, look if the new player is an AI.
     * If so, post an AiWorker to the threadpool. It will later report back
     */
    connect(this, &StateModel::currentPlayerChanged,
            [this]()
    {
        std::shared_ptr<Ai> ai = nullptr;
        switch (mState.turn()) {
        case Stone::Black:
            ai = mBlackAi;
            break;
        case Stone::White:
            ai = mWhiteAi;
            break;
        default:
            break;
        }

        if(ai)
        {
            Q_ASSERT(mState.phase() == Phase::Put || mState.phase() == Phase::Move);
            AiWorker* worker = new AiWorker(ai, mState, this);
            connect(worker, &AiWorker::requestMove, this, &StateModel::processAiMove);
            connect(worker, &AiWorker::requestPut, this, &StateModel::processAiPut);
            QThreadPool::globalInstance()->start(worker);
        }
    });
}

QModelIndex StateModel::index(int row, int column, const QModelIndex &parent) const
{
    if(!hasIndex(row, column, parent))
        return QModelIndex();

    const int* rowPtr = nullptr;

    // if internal ptr = null then the index points to a field
    // if internal ptr = int* then the index points to a stone on a field
    if(parent.isValid())
    {
        Q_ASSERT(parent.internalPointer() == nullptr);
        rowPtr = &mRowTable.at(parent.row());
    }

    return createIndex(row, column, const_cast<int*>(rowPtr));
}

QModelIndex StateModel::parent(const QModelIndex &child) const
{
    if(child.isValid())
    {
        int* rowptr = static_cast<int*>(child.internalPointer());

        if(rowptr)
            return createIndex(*rowptr, 0);
    }

    return QModelIndex();
}

int StateModel::rowCount(const QModelIndex& parent) const
{
    // the root index has the height * width rows because there are that many (possible) fields
    if(!parent.isValid())
        return BoardModel::BOARD_HEIGHT * BoardModel::BOARD_WIDTH;

    int* rowptr = static_cast<int*>(parent.internalPointer());

    const auto& board = BoardModel::Inst();
    if(rowptr)
    {
        // if the internal pointer has a value then the index points to a stone on the second level
        // so there are no children
        return 0;
    } else {
        // the pointer is null. so the parent points to a field in the first level
        // it can have at most one children: when there is a stone on that field
        const auto pos = rowToPosition(parent.row());
        return board.isField(pos) && mState.stoneAt(pos) != Stone::None? 1 : 0;
    }
}

int StateModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return 1;
}


QVariant StateModel::data(const QModelIndex &index, int raw_role) const
{
    if (!index.isValid())
        return QVariant();

    const auto& board = BoardModel::Inst();
    const auto role = StateRole(raw_role);
    const int* rowptr = static_cast<int*>(index.internalPointer());

    if(rowptr)
    {
        const auto pos = rowToPosition(*rowptr);

        if(role == StateRole::Stone)
        {
            return to_string(mState.stoneAt(pos));
        }
    }else{
        const auto pos = rowToPosition(index.row());

        if(role == StateRole::Position)
            return pos;

        bool isField = board.isField(pos);
        if(role == StateRole::IsField)
            return isField;

        if(role == StateRole::IsOccupied)
            return isField && mState.stoneAt(pos) != Stone::None;
    }

    return QVariant();
}

QHash<int, QByteArray> StateModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[(int)StateRole::Position] = "position";
    roles[(int)StateRole::IsOccupied] = "isOccupied";
    roles[(int)StateRole::IsField] = "isField";
    roles[(int)StateRole::Stone] = "stone";
    return roles;
}

void StateModel::initGame(const QString &whiteAi, const QString &blackAi)
{
    auto make_ai = [](const QString& aiName) -> std::shared_ptr<Ai>
    {
        if(aiName == "random")
            return std::make_shared<RandomAi>();
        else if(aiName == "minmax")
            return std::make_shared<MinMaxAi>();
        return nullptr;
    };

    mWhiteAi = make_ai(whiteAi);
    mBlackAi = make_ai(blackAi);

    beginResetModel();
    mState = BoardState();
    endResetModel();

    emit phaseChanged();
    emit currentPlayerChanged();
}

bool StateModel::isValidMove(const QPoint &from, const QPoint &to) const
{
    const auto& board = BoardModel::Inst();
    if(!board.isField(to))
        return false;

    Move move(from, to);
    return mState.isValidMove(move);
}

bool StateModel::isValidMoveEnd(const QPoint &to) const
{
    return isValidMove(getMoveStartPosition(), to);
}

bool StateModel::canRemove(const QPoint &pos) const
{
    const auto& board = BoardModel::Inst();
    if(!board.isField(pos))
        return false;

    Move remove(pos);
    return mState.isValidMove(remove);
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
        return !mWhiteAi;
    } else if(name == to_string(Stone::Black)) {
        return !mBlackAi;
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
    const auto beforePhase = mState.phase();
    Move move(from, to);


    QModelIndex fromIndex = index(positionToRow(from), 0);
    QModelIndex toIndex = index(positionToRow(to), 0);

    beginMoveRows(fromIndex, 0, 0, toIndex, 0);
    mState.move(move);
    qDebug() << "After move";
    qDebug() << mState.toString().toStdString().c_str();
    endMoveRows();

    emit dataChanged(fromIndex, fromIndex);
    emit dataChanged(toIndex, toIndex);

    if(beforePhase != mState.phase())
    {
        emit phaseChanged();
    }

    if(beforeTurn != mState.turn())
    {
        emit currentPlayerChanged();
    }
}

void StateModel::put(const QPoint &to)
{
    const auto beforeTurn = mState.turn();
    const auto beforePhase = mState.phase();

    QModelIndex fieldIndex = index(positionToRow(to), 0);
    beginInsertRows(fieldIndex, 0, 0);
    mState.put(to);
    endInsertRows();
    emit dataChanged(fieldIndex, fieldIndex, {(int)StateRole::IsOccupied});

    if(beforePhase != mState.phase())
    {
        emit phaseChanged();
    }

    if(beforeTurn != mState.turn())
    {
        emit currentPlayerChanged();
    }
    qDebug() << "After put";
    qDebug() << mState.toString().toStdString().c_str();
}

void StateModel::remove(const QPoint &to)
{
    const auto beforeTurn = mState.turn();

    QModelIndex fieldIndex = index(positionToRow(to), 0);
    beginRemoveRows(fieldIndex, 0, 0);
    Move remove(to);
    mState.move(remove);
    endRemoveRows();
    emit dataChanged(fieldIndex, fieldIndex, {(int)StateRole::IsOccupied});
    emit phaseChanged();

    if(beforeTurn != mState.turn())
    {
        emit currentPlayerChanged();
    }

    qDebug() << "After remove";
    qDebug() << mState.toString().toStdString().c_str();
}

void StateModel::processAiPut(Put put)
{
    this->put(put.to());
    if(put.isRemoving())
        this->remove(put.remove());
}

void StateModel::processAiMove(Move move)
{
    this->startMove(move.from());
    this->endMove(move.to());
    if(move.isRemoving())
        this->remove(move.remove());
}

int StateModel::positionToRow(const QPoint &pos) const
{
    return pos.x() + pos.y() * BoardModel::BOARD_WIDTH;
}

QPoint StateModel::rowToPosition(int row) const
{
    return QPoint(row % BoardModel::BOARD_WIDTH, row / BoardModel::BOARD_HEIGHT);
}
