#include "boardmodel.h"

static const int INVALID_FIELD = -1;

const BoardModel &BoardModel::Inst()
{
    static BoardModel model;
    return model;
}

QPoint BoardModel::indexToPosition(int idx) const
{
    return mIndexToPositionMapping.at(idx);
}

int BoardModel::positionToIndex(const QPoint &pos) const
{
    Q_ASSERT(isField(pos));
    return mPositionToIndexMapping.at(pos.x()).at(pos.y());
}

const QVector<int> &BoardModel::adjacentFields(int idx) const
{
    return mAdjacentIndexLists.at(idx);
}

const QVector<QPoint> &BoardModel::adjacentFields(const QPoint &pos) const
{
    return mAdjacentPositionLists.at(positionToIndex(pos));
}

const QVector<QPoint> &BoardModel::fields() const
{
    return mIndexToPositionMapping;
}

const QVector<QPoint> &BoardModel::directions() const
{
    return mDirections;
}

bool BoardModel::isField(const QPoint &pos) const
{
    int x = pos.x();
    int y = pos.y();
    if(x < 0 || BOARD_WIDTH <= x
        || y < 0 || BOARD_HEIGHT <= y)
        return false;
    return mPositionToIndexMapping[x][y] != INVALID_FIELD;
}

BoardModel::BoardModel()
    : mDirections{NORTH, EAST, SOUTH, WEST}
    , mPositionToIndexMapping(BOARD_WIDTH, QVector<int>(BOARD_HEIGHT, INVALID_FIELD))
    , mAdjacentMatrix(BOARD_WIDTH, QVector<bool>(BOARD_HEIGHT, false))
    , mAdjacentIndexLists(BOARD_FIELDS_NUM)
    , mAdjacentPositionLists(BOARD_FIELDS_NUM)
{
    /*
     * Template of the board
     * X = a field, where a stone can be put
     * + = a connection between two X
     */
    const QVector<QString> board_template = {
        "X++X++X",
        "+X+X+X+",
        "++XXX++",
        "XXX XXX",
        "++XXX++",
        "+X+X+X+",
        "X++X++X"
    };

    // build mappings
    int idx = 0;
    for(int y = 0; y < BOARD_HEIGHT; ++y)
    {
        for(int x = 0; x < BOARD_WIDTH; ++x)
        {

            bool isField = board_template[x][y] == 'X';

            if(isField) {
                mPositionToIndexMapping[x][y] = idx;
                mIndexToPositionMapping.push_back(QPoint(x, y));
                ++idx;
            }
        }
    }

    const auto make_adjacent = [this](const QPoint& from, const QPoint& to) {
        const int from_idx = positionToIndex(from);
        const int to_idx = positionToIndex(to);

        mAdjacentMatrix[from.x()][from.y()] = true;
        mAdjacentIndexLists[from_idx].push_back(to_idx);
        mAdjacentPositionLists[from_idx].push_back(to);
    };

    Q_ASSERT(mIndexToPositionMapping.size() == BOARD_FIELDS_NUM);

    // build adjacent lists and matrix
    for(const QPoint& from: fields())
    {
        // search in all directions to find an adjacent field
        for(const QPoint& dir: directions())
        {
            for(QPoint to = from + dir;
                isField(to);
                to += dir)
            {
                auto field = board_template[to.x()][to.y()];

                // only continue into the direction if '+' was found
                if(field == '+')
                    continue;

                if(field == 'X')
                    make_adjacent(from, to);

                break;
            }
        }
    }
}
