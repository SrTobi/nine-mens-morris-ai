#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <statemodel.h>
#include <boardmodel.h>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    BoardState state;
    state.setStoneAt(3, Stone::Black);
    state.setStoneAt(4, Stone::White);
    StateModel stateModel(state, true, true);
    BoardModel& boardModel = BoardModel::Inst();
    auto* ctx = engine.rootContext();
    ctx->setContextProperty("stateModel", &stateModel);
    ctx->setContextProperty("boardModel", &boardModel);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
