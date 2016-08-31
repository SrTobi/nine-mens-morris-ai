#ifndef BOARDSTATE_H
#define BOARDSTATE_H

#include <array>

#include <QObject>
#include <QAbstractItemModel>
#include <functional>
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


    int whiteStones() const;
    int blackStones() const;
    int stonesOf(Stone stone) const;

    bool isValidMove(const Move& move) const;
    void move(const Move& move);

    bool isValidPut(const Put& put) const;
    void put(int idx);
    void put(const QPoint& pos);
    void put(const Put& put);

    Stone millAt(int idx) const;
    Stone millAt(const QPoint& pos) const;

    void generateMoves(const std::function<bool(const Move& move)>& callback) const;
    void generatePuts(const std::function<bool(const Put& put)>& callback) const;

    Stone turn() const;
    Stone opponent() const;

    Phase phase() const;
    bool hasEnded() const;
    Stone winner() const;
    Stone looser() const;

    QString toString() const;
private:
    void setStoneAt(int idx, Stone stone);
    void setStoneAt(const QPoint& pos, Stone stone);

private:
    std::array<Stone, BoardModel::BOARD_FIELDS_NUM> mFields;
    Stone mTurn;
    Phase mPhase;
    uint16_t mPutStones;
};

#endif // BOARDSTATE_H
