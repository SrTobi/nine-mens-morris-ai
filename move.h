#ifndef MOVE_H
#define MOVE_H

#include <QPoint>
#include <QObject>
#include "stone.h"


/*
 * Represents a move operation which can be used on a BoardState
 * It contains a move (i.e. moving a stone from one position to another)
 * and/or a remove (i.e. removing an opponents stone).
 */
class Move
{
public:
    Move();
    Move(int from, int to);
    Move(int remove);
    Move(int from, int to, int remove);
    Move(const QPoint& from, const QPoint& to);
    Move(const QPoint& remove);
    Move(const QPoint& from, const QPoint& to, const QPoint& remove);

    int fromIdx() const;
    int toIdx() const;
    int removeIdx() const;
    QPoint from() const;
    QPoint to() const;
    QPoint remove() const;

    bool isValid() const;
    bool isRemoving() const;
    bool isMoving() const;

private:
    int mFrom;
    int mTo;
    int mRemove;
};
Q_DECLARE_METATYPE(Move)

/*
 * Represents a put operation which can be used on a BoardState
 * It contains a put (i.e. putting a stone on a position)
 * and optionally an additional remove (i.e. removing an opponents stone).
 */
class Put
{
public:
    Put();
    Put(int to);
    Put(int to, int remove);
    Put(const QPoint& to);
    Put(const QPoint& to, const QPoint& remove);

    int toIdx() const;
    int removeIdx() const;
    QPoint to() const;
    QPoint remove() const;

    bool isValid() const;
    bool isRemoving() const;
private:
    int mTo;
    int mRemove;
};
Q_DECLARE_METATYPE(Put)
#endif // MOVE_H
