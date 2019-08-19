#include "AppMain.hpp"

AppMain::AppMain(QObject *parent) : QObject(parent)
{
    m_mainController = new MainController(this);
}

void AppMain::initApplication(QQmlApplicationEngine &engine)
{
    LOG_DEBUG;

    // Coppy icon to /SDCARD/DCIM folder
    if(!QDir("/sdcard/DCIM/PDT17/Icons").exists()){
        if(QDir("/sdcard/DCIM/").mkdir("PDT17")){
            QDir("/sdcard/DCIM/PDT17").mkdir("Icons");
        }
    }
    this->coppyFolder("assets:/images/Icons","/sdcard/DCIM/PDT17/Icons");

    // Init Model
    Model::instance()->initModel();

    // Init Main controller
    m_mainController->initController();

    //Set context property
    engine.rootContext()->setContextProperty("APP_MODEL", Model::instance());

    //Register Enum class
    qmlRegisterType<AppEnums>("AppEnums", 1, 0, "AppEnums");

}

void AppMain::coppyFolder(QString src, QString dst)
{
    QDir dir(src);
    if (! dir.exists()){
        LOG_DEBUG << "src folder not exist";
        return;
    }

    foreach (QString d, dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
        QString dst_path = dst + QDir::separator() + d;
        dir.mkpath(dst_path);
        coppyFolder(src+ QDir::separator() + d, dst_path);
    }

    foreach (QString f, dir.entryList(QDir::Files)) {
        QFile::copy(src + QDir::separator() + f, dst + QDir::separator() + f);
    }
}

