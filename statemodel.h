#ifndef STATEMODEL_H
#define STATEMODEL_H

#include <QAbstractListModel>
#include "boardstate.h"

class StateModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QString currentPlayer READ currentPlayer NOTIFY currentPlayerChanged)
    Q_PROPERTY(bool isMoving READ isMoving NOTIFY isMovingChanged)
    Q_PROPERTY(QString phase READ phase NOTIFY phaseChanged)
public:
    enum class StateRole {
        Position = Qt::UserRole + 1,
        IsOccupied,
        Stone
    };

public:
    explicit StateModel(const BoardState& state, bool whitePlayerMovable, bool blackPlayerMovable);


    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;


    QHash<int,QByteArray> roleNames() const override;


    //Q_INVOKABLE QPoint boardIndexToPosition(int index) const;
    // Editable:
    /*bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;*/
    Q_INVOKABLE const QString& currentPlayer() const;
    Q_INVOKABLE const QString& phase() const;
    Q_INVOKABLE bool isValidMove(const QPoint& from, const QPoint& to) const;
    Q_INVOKABLE bool isValidMoveEnd(const QPoint& to) const;
    Q_INVOKABLE QPoint getMoveStartPosition() const;
    Q_INVOKABLE bool isMoving() const;
    Q_INVOKABLE bool isMovablePlayer(const QString& name) const;
public slots:
    void startMove(const QPoint& from);
    void abortMove();
    void endMove(const QPoint& to);
    void put(const QPoint& to);
    void remove(const QPoint& to);

signals:
    void currentPlayerChanged();
    void isMovingChanged();
    void phaseChanged();

private:
    QScopedPointer<QPoint> mMoveStart;
    BoardState mState;
    const bool mBlackMovable;
    const bool mWhiteMovable;
};

#endif // STATEMODEL_H
