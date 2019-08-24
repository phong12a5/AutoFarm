#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <AppDefines.hpp>
#include "Model.hpp"
#include "AppMain.hpp"

int main(int argc, char *argv[])
{
    LOG_DEBUG << "STARTING .... ";
    LOG_DEBUG << "Thread ID: " << QThread::currentThreadId();

    LOG_DEBUG <<  "supportsSsl: " << QSslSocket::supportsSsl();

    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;


    AppMain appMain;
    appMain.initApplication(engine);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty()){
        LOG_DEBUG << "rootObject is NULL";
    }


    return app.exec();
}

