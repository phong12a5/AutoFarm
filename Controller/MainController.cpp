#include "MainController.h"

MainController* MainController::m_instance = nullptr;

MainController::MainController(QObject *parent) : QObject(parent)
{
    m_currentScreen = -1;
    m_currentActivity = "";
}

MainController *MainController::instance()
{
    if(m_instance == nullptr){
        m_instance = new MainController();
    }
    return m_instance;
}

void MainController::initController()
{
    LOG;
    // Do nothing;
    connect(this,SIGNAL(currentScreenChanged()),this,SLOT(onChangeScreen()));
    connect(this,SIGNAL(currentActivityChanged()),this,SLOT(onChangeAcitivity()));
}

void MainController::startLoop()
{
    LOG;
    m_webAPI.installAllPackages();
    QEventLoop evenLoop;
    connect(&m_webAPI, SIGNAL(installAllPackagesCompleted()), &evenLoop, SLOT(quit()));
    evenLoop.exec();
    startCheckCurrentActivity();
}

int MainController::currentScreen() const
{
    return m_currentScreen;
}

void MainController::setCurrentScreen(const int data)
{
    if(m_currentScreen != data){
        LOG << data;
        m_currentScreen = data;
        emit currentScreenChanged();
    }
}

QString MainController::currentActivity() const
{
    return m_currentActivity;
}

void MainController::setCurrentActivity(const QString data)
{
    if(m_currentActivity != data){
        LOG << data;
        m_currentActivity = data;
        emit currentActivityChanged();
    }
}

void MainController::startCheckCurrentScreen()
{
    LOG;
    multiThreadController.startCheckCurrentScreen();
}

void MainController::startCheckCurrentActivity()
{
    multiThreadController.startCheckCurrentActivity();
}

void MainController::onChangeScreen()
{
    LOG;
}

void MainController::onChangeAcitivity()
{
    LOG;
    if(currentActivity() == FACEBOOK_LITE_SCREEN){
        this->startCheckCurrentScreen();
        delay(1000);
    }else{
#ifdef ANDROID_KIT
        JavaCommunication::instance()->openApplication(FACEBOOK_LITE_PKGNAME,FACEBOOK_LITE_ACT);
#endif
    }
}
