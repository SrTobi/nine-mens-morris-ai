#ifndef BOARDSTATE_H
#define BOARDSTATE_H

#include <array>

#include <QObject>
#include <QAbstractItemModel>
#include "boardmodel.h"

enum class Stone: unsigned char {
    None,
    Black,
    White
};

Q_ENUMS(Stone);

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

    Stone turn() const;

signals:

public slots:

private:
    std::array<Stone, BoardModel::BOARD_FIELDS_NUM> mFields;
    Stone mTurn;
};

#endif // BOARDSTATE_H
