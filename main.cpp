#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <AppDefines.h>
#include "Model.h"
#include "Controller/ShellOperation.h"
#include "Processing/ImageProcessing.h"
#include "AppMain.h"

int main(int argc, char *argv[])
{
    LOG << "STARTING ....";

    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    AppMain appMain;
    appMain.initApplication(engine);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty()){
        LOG << "rootObject is NULL";
    }

    JavaCommunication::instance()->openFBLiteWithUserID("com.facebook.lizh","vongocanh.vo");

    return app.exec();
}

