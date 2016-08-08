#ifndef STATEMODEL_H
#define STATEMODEL_H

#include <QAbstractListModel>
#include "boardstate.h"

class StateModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QString currentPlayer READ currentPlayer NOTIFY currentPlayerChanged)
    Q_PROPERTY(bool isMoving READ isMoving NOTIFY isMovingChanged)
public:
    enum class StateRole {
        Position = Qt::UserRole + 1,
        IsOccupied,
        Stone
    };

public:
    explicit StateModel(const BoardState& state);


    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;


    QHash<int,QByteArray> roleNames() const override;


    //Q_INVOKABLE QPoint boardIndexToPosition(int index) const;
    // Editable:
    /*bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;*/

    Q_INVOKABLE bool isValidMove(const QPoint& from, const QPoint& to) const;
    Q_INVOKABLE bool isValidMoveEnd(const QPoint& to) const;
    Q_INVOKABLE QPoint getMoveStartPosition() const;
    const QString& currentPlayer() const;
    bool isMoving() const;
public slots:
    void startMove(const QPoint& from);
    void abortMove();
    void endMove(const QPoint& to);

signals:
    void currentPlayerChanged();
    void isMovingChanged();

private:
    QScopedPointer<QPoint> mMoveStart;
    BoardState mState;
};

#endif // STATEMODEL_H
