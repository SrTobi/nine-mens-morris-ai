#ifndef BOARDMODEL_H
#define BOARDMODEL_H

#include <QObject>
#include <QVector>
#include <QPoint>

class BoardModel : public QObject
{
    Q_GADGET
public:
    static const int BOARD_FIELDS_NUM = 24;
    static const int BOARD_WIDTH = 7;
    static const int BOARD_HEIGHT = 7;
    static constexpr QPoint NORTH = QPoint(0, -1);
    static constexpr QPoint EAST =  QPoint(1, 0);
    static constexpr QPoint SOUTH = QPoint(0, 1);
    static constexpr QPoint WEST =  QPoint(-1, 0);

public:
    static BoardModel& Inst();


    Q_INVOKABLE QPoint indexToPosition(int idx) const;
    Q_INVOKABLE int positionToIndex(const QPoint& pos) const;

    Q_INVOKABLE const QVector<int> &adjacentFields(int idx) const;
    const QVector<QPoint>& adjacentFields(const QPoint& pos) const;

    const QVector<QPoint>& fields() const;

    const QVector<QPoint>& directions() const;
    Q_INVOKABLE bool isField(const QPoint& pos) const;
private:
    BoardModel();
    BoardModel(const BoardModel&) = delete; // non construction-copyable
    BoardModel& operator=(const BoardModel&) = delete; // non copyable

    const QVector<QPoint> mDirections;
    QVector<QVector<int>> mPositionToIndexMapping;
    QVector<QPoint> mIndexToPositionMapping;

    QVector<QVector<bool>> mAdjacentMatrix;
    QVector<QVector<int>> mAdjacentIndexLists;
    QVector<QVector<QPoint>> mAdjacentPositionLists;
};

#endif // BOARDMODEL_H
