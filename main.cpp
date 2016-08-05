#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <statemodel.h>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    BoardState state;
    state.setStoneAt(3, Stone::Black);
    state.setStoneAt(4, Stone::White);
    StateModel model(state);
    auto* ctx = engine.rootContext();
    ctx->setContextProperty("myModel", &model);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
