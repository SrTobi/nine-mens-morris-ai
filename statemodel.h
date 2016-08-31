#ifndef STATEMODEL_H
#define STATEMODEL_H

#include <QAbstractItemModel>
#include <vector>
#include <memory>
#include "ai.h"
#include "boardstate.h"

/*
 * The StateModel is the mediator between the current BoardState and the view
 * It mimics the current state of the board with an AbstractItemModel
 * so that the view can show the board as a hirachical item model.
 * Various properties and functions allow the view to present the current state of the game.
 *
 * The first level of the model contains all fields (and non fields).
 * The second level contains at most one stone (that lies on the field)
 * The abstract item model works only with rows so that positions have to be mapped to rows.
 *
 * Model example:
 * + root
 *   |
 *   + Field(0,0)
 *   |
 *   + Field(0,1
 *   |
 *   + Field(0,2)
 *   |
 *   + Field(0,3)
 *   | |
 *   | + Stone(Black)
 *   |
 *   + Field(0,4)
 *   |
 *   + Field(0,5)
 *   |
 *  ...
 *
 */
class StateModel : public QAbstractItemModel
{
    Q_OBJECT
    Q_PROPERTY(QString currentPlayer READ currentPlayer NOTIFY currentPlayerChanged)
    Q_PROPERTY(bool isMoving READ isMoving NOTIFY isMovingChanged)
    Q_PROPERTY(QString phase READ phase NOTIFY phaseChanged)

public:
    /*
     * Every possible field in the model (the first model level)
     * can access multiple values, containing more information about the field
     */
    enum class StateRole {
        Position = Qt::UserRole + 1,
        IsOccupied,
        IsField,
        Stone
    };

public:
    explicit StateModel(const BoardState& state, const std::shared_ptr<Ai>& whiteAi, const std::shared_ptr<Ai>& blackAi);

    QModelIndex parent(const QModelIndex &child) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int,QByteArray> roleNames() const override;

    Q_INVOKABLE void initGame(const QString& whiteAi, const QString& blackAi);
    Q_INVOKABLE const QString& currentPlayer() const;
    Q_INVOKABLE const QString& phase() const;
    Q_INVOKABLE bool isValidMove(const QPoint& from, const QPoint& to) const;
    Q_INVOKABLE bool isValidMoveEnd(const QPoint& to) const;
    Q_INVOKABLE bool canRemove(const QPoint& pos) const;
    Q_INVOKABLE QPoint getMoveStartPosition() const;
    Q_INVOKABLE bool isMoving() const;
    Q_INVOKABLE bool isMovablePlayer(const QString& name) const;
public slots:
    /*
     * Starts a new drag and drop move
     * Especially affects isValidMoveEnd
     */
    void startMove(const QPoint& from);
    void abortMove();
    /*
     * Ends a move and adjusts the model
     */
    void endMove(const QPoint& to);
    void put(const QPoint& to);
    void remove(const QPoint& to);
    void processAiPut(Put put);
    void processAiMove(Move move);

signals:
    void currentPlayerChanged();
    void isMovingChanged();
    void phaseChanged();

private:
    int positionToRow(const QPoint& pos) const;
    QPoint rowToPosition(int row) const;
private:
    QScopedPointer<QPoint> mMoveStart;
    BoardState mState;
    std::shared_ptr<Ai> mWhiteAi;
    std::shared_ptr<Ai> mBlackAi;

    std::vector<int> mRowTable;
};

#endif // STATEMODEL_H
