#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <statemodel.h>
#include <boardmodel.h>


class RandomAi: public Ai
{
public:
    Move nextMove(const BoardState& state)
    {
        Move move;
        state.generateMoves([&](const Move& m)
        {
            move = m;
            return rand() % 3 > 0;
        });
        return move;
    }

    Put nextPut(const BoardState &state)
    {
        Put put;
        state.generatePuts([&](const Put& p)
        {
            put = p;
            return rand() % 3 > 0;
        });

        return put;
    }
};

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    qRegisterMetaType<Move>();
    qRegisterMetaType<Put>();

    BoardState state;
    //state.setStoneAt(3, Stone::Black);
    //state.setStoneAt(4, Stone::White);
    /*for(int i = 0; i < 16; ++i)
        state.put(i);//*/
    StateModel stateModel(state, nullptr, std::unique_ptr<RandomAi>(new RandomAi()));
    BoardModel& boardModel = BoardModel::Inst();
    auto* ctx = engine.rootContext();
    ctx->setContextProperty("stateModel", &stateModel);
    ctx->setContextProperty("boardModel", &boardModel);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
