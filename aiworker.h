#ifndef AIWORKER_H
#define AIWORKER_H

#include <QRunnable>
#include "statemodel.h"


/*
 * The AiWorker executes the AI and reports the resulting move/put to the stateModel
 */
class AiWorker: public QObject, public QRunnable
{
    Q_OBJECT
public:
    AiWorker(std::shared_ptr<Ai> ai, const BoardState& state, StateModel* stateModel);

    void run() override;

signals:
    /*
     * The ai calculated a put and requests to change the model accordingly
     */
    void requestPut(Put put);

    /*
     * The ai calculated a move and requests to change the model accordingly
     */
    void requestMove(Move move);

private:
    std::shared_ptr<Ai> mAi;
    BoardState mState;
    StateModel* mStateModel;

};
#endif // AIWORKER_H
