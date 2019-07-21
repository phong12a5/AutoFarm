#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <AppDefines.hpp>
#include "Model.hpp"
#include "Controller/ShellOperation.hpp"
#include "Processing/ImageProcessing.hpp"
#include "AppMain.hpp"
#include <QPoint>

int main(int argc, char *argv[])
{
    LOG << "STARTING .... ";
    LOG << "Thread ID: " << QThread::currentThreadId();

    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

#ifdef SU_USER
    while(QProcess::execute("su -c ls") != 0){
        delay(10000);
        LOG << "Request Superuser again!";
    }
#endif
    LOG << "Superuser is granted!";

    AppMain appMain;
    appMain.initApplication(engine);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty()){
        LOG << "rootObject is NULL";
    }

    return app.exec();
}

