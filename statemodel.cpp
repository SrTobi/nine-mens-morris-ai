#include "statemodel.h"
#include "boardmodel.h"

StateModel::StateModel(const BoardState& state)
    : mState(state)
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
