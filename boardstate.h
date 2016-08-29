#ifndef BOARDSTATE_H
#define BOARDSTATE_H

#include <array>

#include <QObject>
#include <QAbstractItemModel>
#include "stone.h"
#include "boardmodel.h"

enum class Phase: unsigned char {
    Put,
    Move,
    Remove,
    PutRemove,
    End
};

const QString& to_string(Phase phase);

//Q_ENUMS(Stone);

class BoardState
{
public:
    explicit BoardState();

    Stone stoneAt(int idx) const;
    Stone stoneAt(const QPoint& pos) const;

    void setStoneAt(int idx, Stone stone);
    void setStoneAt(const QPoint& pos, Stone stone);

    int whiteStones() const;
    int blackStones() const;
    int stonesOf(Stone stone) const;

    Stone turn() const;
    Phase phase() const;
private:
    std::array<Stone, BoardModel::BOARD_FIELDS_NUM> mFields;
    Stone mTurn;
    Phase mPhase;
};

#endif // BOARDSTATE_H
