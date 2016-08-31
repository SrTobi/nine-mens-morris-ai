#ifndef AIWORKER_H
#define AIWORKER_H

#include <QRunnable>
#include "statemodel.h"



class AiWorker: public QObject, public QRunnable
{
    Q_OBJECT
public:
    AiWorker(std::shared_ptr<Ai> ai, const BoardState& state, StateModel* stateModel);

    void run() override;

signals:
    void requestPut(Put put);
    void requestMove(Move move);

private:
    std::shared_ptr<Ai> mAi;
    BoardState mState;
    StateModel* mStateModel;

};
#endif // AIWORKER_H
