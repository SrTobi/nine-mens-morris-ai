#include "boardmodel.h"

static const int INVALID_FIELD = -1;

BoardModel &BoardModel::Inst()
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

const QVector<QPoint>& BoardModel::adjacentFields(const QPoint &pos) const
{
    return mAdjacentPositionLists.at(positionToIndex(pos));
}

const QVector<int>& BoardModel::fieldIndices() const
{
    return mFieldIndices;
}

const QVector<QPoint> &BoardModel::fields() const
{
    return mIndexToPositionMapping;
}

const QVector<QPoint> &BoardModel::directions() const
{
    return mDirections;
}

bool BoardModel::isOnBoard(int idx) const
{
    return 0 <= idx && idx < BOARD_FIELDS_NUM;
}

bool BoardModel::isOnBoard(const QPoint &pos) const
{
    int x = pos.x();
    int y = pos.y();
    return 0 <= x && x < BOARD_WIDTH
            && 0 <= y && y < BOARD_HEIGHT;
}

bool BoardModel::isField(int idx) const
{
    return isOnBoard(idx);
}

bool BoardModel::isField(const QPoint &pos) const
{
    int x = pos.x();
    int y = pos.y();
    return isOnBoard(pos)
            && mPositionToIndexMapping[x][y] != INVALID_FIELD;
}

bool BoardModel::areAdjacentFields(const QPoint &p1, const QPoint &p2) const
{
    return areAdjacentFields(positionToIndex(p1), positionToIndex(p2));
}

bool BoardModel::areAdjacentFields(int i1, int i2) const
{
    return mAdjacentMatrix[i1][i2];
}

/*QPoint BoardModel::boardIndexToPosition(int index) const
{
    return QPoint(index % BOARD_WIDTH, index / BOARD_WIDTH);
}*/

BoardModel::BoardModel()
    : mDirections{NORTH, EAST, SOUTH, WEST}
    , mPositionToIndexMapping(BOARD_WIDTH, QVector<int>(BOARD_HEIGHT, INVALID_FIELD))
    , mAdjacentMatrix(BOARD_FIELDS_NUM, QVector<bool>(BOARD_FIELDS_NUM, false))
    , mAdjacentIndexLists(BOARD_FIELDS_NUM)
    , mAdjacentPositionLists(BOARD_FIELDS_NUM)
{
    for(int i = 0; i < BOARD_FIELDS_NUM; ++i)
        mFieldIndices.push_back(i);
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

        mAdjacentMatrix[from_idx][to_idx] = true;
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
                isOnBoard(to);
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
