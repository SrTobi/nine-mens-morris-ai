#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "statemodel.h"
#include "boardmodel.h"
#include "randomai.h"


int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    qRegisterMetaType<Move>();
    qRegisterMetaType<Put>();

    BoardState state;
    StateModel stateModel(state, nullptr, std::unique_ptr<RandomAi>(new RandomAi()));
    BoardModel& boardModel = BoardModel::Inst();
    auto* ctx = engine.rootContext();
    ctx->setContextProperty("stateModel", &stateModel);
    ctx->setContextProperty("boardModel", &boardModel);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
