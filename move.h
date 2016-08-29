#ifndef MOVE_H
#define MOVE_H

#include <QPoint>
#include "stone.h"

class Move
{
public:
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

    bool isRemoving() const;
    bool isMoving() const;

private:
    const int mFrom;
    const int mTo;
    const int mRemove;
};

#endif // MOVE_H
