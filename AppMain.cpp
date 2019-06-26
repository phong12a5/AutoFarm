#include "AppMain.h"

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
#endif
    //Connect  signal - slot
    this->connectSignalSlot();
}

void AppMain::connectSignalSlot() const
{
    LOG;
    connect(MODEL,SIGNAL(sigStartProgram()),this,SLOT(onStartProgram()));
}

void AppMain::onStartProgram()
{
    LOG;
    MAIN_CONTROLLER->startLoop();
}
