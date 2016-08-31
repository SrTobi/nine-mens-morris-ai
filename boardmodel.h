#ifndef BOARDMODEL_H
#define BOARDMODEL_H

#include <QObject>
#include <QVector>
#include <QPoint>
#include <array>

/*
 * A constant static model of a nine men's morris board
 * It contains various functions to convert positions to indices.
 */
class BoardModel : public QObject
{
    Q_OBJECT
public:
    /*
     *  Number of fields where stones can be placed upon
     */
    static const int BOARD_FIELDS_NUM = 24;

    /*
     *  Width of the board
     */
    static const int BOARD_WIDTH = 7;

    /*
     *  Height of the board
     */
    static const int BOARD_HEIGHT = 7;

    /*
     *  Number of fields including positions where no stone can be placed
     */
    static const int BOARD_DIM = BOARD_WIDTH * BOARD_HEIGHT;

    /*
     *  Number of stones that allow to fly
     */
    static const int NUM_STONES_ALLOW_FLY = 3;

    /*
     *  Number of stones each player can put in the put phase
     */
    static const int NUM_STONES_TO_PUT = 9;


    const QPoint NORTH = QPoint(0, -1);
    const QPoint EAST =  QPoint(1, 0);
    const QPoint SOUTH = QPoint(0, 1);
    const QPoint WEST =  QPoint(-1, 0);

public:
    /*
     * Return the board singleton.
     *
     * It can not be const, because const is not supported by QQmlContext::setContextProperty
     */
    static BoardModel& Inst();


    Q_INVOKABLE QPoint indexToPosition(int idx) const;
    Q_INVOKABLE int positionToIndex(const QPoint& pos) const;

    Q_INVOKABLE const QVector<int> &adjacentFields(int idx) const;
    const QVector<QPoint>& adjacentFields(const QPoint& pos) const;

    const QVector<int>& fieldIndices() const;
    const QVector<QPoint>& fields() const;
    const QVector<std::array<int, 3>> possibleMills() const;

    const QVector<QPoint>& directions() const;
    Q_INVOKABLE bool isOnBoard(int idx) const;
    Q_INVOKABLE bool isOnBoard(const QPoint& pos) const;
    Q_INVOKABLE bool isField(int idx) const;
    Q_INVOKABLE bool isField(const QPoint& pos) const;
    Q_INVOKABLE bool areAdjacentFields(const QPoint& p1, const QPoint& p2) const;
    Q_INVOKABLE bool areAdjacentFields(int i1, int i2) const;
private:
    BoardModel();
    BoardModel(const BoardModel&) = delete; // non construction-copyable
    BoardModel& operator=(const BoardModel&) = delete; // non copyable

    QVector<int> mFieldIndices;
    const QVector<QPoint> mDirections;
    QVector<QVector<int>> mPositionToIndexMapping;
    QVector<QPoint> mIndexToPositionMapping;

    QVector<QVector<bool>> mAdjacentMatrix;
    QVector<QVector<int>> mAdjacentIndexLists;
    QVector<QVector<QPoint>> mAdjacentPositionLists;

    QVector<std::array<int, 3>> mPossibleMills;
};

#endif // BOARDMODEL_H
