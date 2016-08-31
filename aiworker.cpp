#include <QTime>
#include <QThread>
#include "aiworker.h"

AiWorker::AiWorker(std::shared_ptr<Ai> ai, const BoardState &state, StateModel *stateModel)
    : mAi(ai)
    , mState(state)
    , mStateModel(stateModel)
{
}

void AiWorker::run()
{
    QTime timer;

    auto wait = [&timer]()
    {
        const int waittime = 500;
        if(timer.elapsed() < waittime)
            QThread::msleep(waittime - timer.elapsed());
    };

    if(mState.phase() == Phase::Put)
    {
        Put put = mAi->nextPut(mState);
        wait();
        emit requestPut(put);

    }else{
        Move move = mAi->nextMove(mState);
        wait();
        emit requestMove(move);
    }
}
