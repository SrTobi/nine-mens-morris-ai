#ifndef STATEMODEL_H
#define STATEMODEL_H

#include <QAbstractListModel>
#include "boardstate.h"

class StateModel : public QAbstractListModel
{
    Q_GADGET
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


    // Editable:
    /*bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;*/

private:
    BoardState mState;
};

#endif // STATEMODEL_H
