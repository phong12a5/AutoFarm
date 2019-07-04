#include "AppMain.hpp"

#define MODEL           Model::instance()
#define MAIN_CONTROLLER MainController::instance()
#define JAVA_COM        JavaCommunication::instance()

AppMain::AppMain(QObject *parent) : QObject(parent)
{
    LOG;
}

void AppMain::initApplication(QQmlApplicationEngine &engine)
{
    LOG;

    // Remove notification as user root
    ShellOperation::removeFile("/system/app/SuperSU/SuperSU.apk");

    // Coppy icon to /SDCARD/DCIM folder
    if(!QDir("/sdcard/DCIM/PDT17/Icons").exists()){
        if(QDir("/sdcard/DCIM/").mkdir("PDT17")){
            QDir("/sdcard/DCIM/PDT17").mkdir("Icons");
        }
    }

    this->coppyFolder("assets:/images/Icons","/sdcard/DCIM/PDT17/Icons");

    // Init Main controller
    MAIN_CONTROLLER->initController();

    //Set context property
    engine.rootContext()->setContextProperty("APP_MODEL",MODEL);

    //Register Enum class
    qmlRegisterType<AppEnums>("AppEnums", 1, 0, "AppEnums");

#ifdef ANDROID_KIT
    DEVICE_INFO _deviceInfo;
    _deviceInfo.googleSF = JAVA_COM->getGoogleSF();
    _deviceInfo.androidID = JAVA_COM->getAndroidID();
    _deviceInfo.iMEI = JAVA_COM->getDeviceIMEI();
    _deviceInfo.iMSI = JAVA_COM->getDeiceIMSI();
    _deviceInfo.sIMCardSerial = JAVA_COM->getSimCardSerialNo();
    _deviceInfo.wifiMacAddress = JAVA_COM->getWifiMacAdd();
    _deviceInfo.android_verion = JAVA_COM->getAndroidVersion();
    _deviceInfo.model = JAVA_COM->getDeviceModel();
    _deviceInfo.isNox = JAVA_COM->getDeviceType();
    MODEL->setDeviceInfo(_deviceInfo);
    _deviceInfo.disInfo = ShellOperation::getDisplayInfo();
    MODEL->setDeviceInfo(_deviceInfo);

    LOG << _deviceInfo.googleSF;
    LOG << _deviceInfo.androidID;
    LOG << _deviceInfo.iMEI;
    LOG << _deviceInfo.iMSI;
    LOG << _deviceInfo.sIMCardSerial;
    LOG << _deviceInfo.wifiMacAddress;
    LOG << _deviceInfo.android_verion;
    LOG << _deviceInfo.model;
    LOG << _deviceInfo.isNox;
    LOG << _deviceInfo.disInfo.width;
    LOG << _deviceInfo.disInfo.height;
    LOG << _deviceInfo.disInfo.dpi;
#endif

    //Connect  signal - slot
    this->connectSignalSlot();
}

void AppMain::connectSignalSlot() const
{
    LOG;
    connect(MODEL,SIGNAL(sigStartProgram()),this,SLOT(onStartProgram()));
}

void AppMain::coppyFolder(QString src, QString dst)
{
    QDir dir(src);
    if (! dir.exists()){
        LOG << "src folder not exist";
        return;
    }

    foreach (QString d, dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
        QString dst_path = dst + QDir::separator() + d;
        dir.mkpath(dst_path);
        coppyFolder(src+ QDir::separator() + d, dst_path);
    }

    foreach (QString f, dir.entryList(QDir::Files)) {
        LOG << "coppying " << src + QDir::separator() + f << " to " << dst + QDir::separator() + f;
        QFile::copy(src + QDir::separator() + f, dst + QDir::separator() + f);
    }
}

void AppMain::onStartProgram()
{
    LOG;
    MAIN_CONTROLLER->startLoop();
}
