#ifndef BOARDSTATE_H
#define BOARDSTATE_H

#include <array>

#include <QObject>
#include <QAbstractItemModel>
#include "stone.h"
#include "move.h"
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

    bool isValidMove(const Move& move);
    void move(const Move& move);

    void put(int idx);
    void put(const QPoint& pos);

    Stone millAt(int idx);
    Stone millAt(const QPoint& pos);

    Stone turn() const;
    Stone opponent() const;

    Phase phase() const;
    bool hasEnded() const;
    Stone winner() const;
    Stone looser() const;
private:
    std::array<Stone, BoardModel::BOARD_FIELDS_NUM> mFields;
    Stone mTurn;
    Phase mPhase;
};

#endif // BOARDSTATE_H
