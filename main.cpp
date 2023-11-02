#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickWindow>
#include <QSGRendererInterface>
#include <QCoreApplication>
#include <QDir>
#include <QQmlContext>


#include<QDebug>

#include"neuralnetworkwraper.h"

int main(int argc, char *argv[])
{
    QQuickWindow::setGraphicsApi(QSGRendererInterface::Software); //stable work on every system
    QGuiApplication app(argc, argv);


    QQmlApplicationEngine engine;

    QDir dir(QCoreApplication::applicationDirPath());
    std::string path = dir.absolutePath().toStdString()+ QString(QDir::separator()).toStdString() + "GoodSetPerfectStudying.txt";

    NeuralNetworkWraper nWraper;
    nWraper.network.ReadWeightsAndBayeses(path);

    engine.rootContext()->setContextProperty("neural",&nWraper);

    app.setWindowIcon(QIcon(":/images/Neural.png"));


    const QUrl url(u"qrc:/Neurons/Main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
        &app, []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
