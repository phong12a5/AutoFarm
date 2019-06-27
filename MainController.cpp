#include "MainController.h"
#include "Communication/WebAPI.h"
#include "Model.h"

#define WEB_API WebAPI::instance()
#define MODEL Model::instance()
#define JAVA_COM JavaCommunication::instance()

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
    connect(MODEL,SIGNAL(nextCurrentControlledObjChanged()),this,SLOT(executeRequiredActions()));
    connect(MODEL,SIGNAL(currentActionChanged()),this,SLOT(doAction()));
}

void MainController::startLoop()
{
    LOG;
    MODEL->setUserData(WEB_API->cloneUserData());
    WEB_API->getApk();
    WEB_API->installAllPackages();
    QEventLoop evenLoop;
    connect(WEB_API, SIGNAL(installAllPackagesCompleted()), &evenLoop, SLOT(quit()));
    evenLoop.exec();
    MODEL->nextCurrentControlledObj();
//    for(int i = 0; i < MODEL->getUserDataList().count(); i++){
//        MODEL->currentControlledUser() = WEB_API->cloneUserData();
//        MODEL->saveUserDataList();
//    }
    //    startCheckCurrentActivity();
}

void MainController::execVipLike()
{
    LOG;
    if(MODEL->currentAction()["fbid"].toString() != ""){
        JAVA_COM->openFBLiteWithUserID(MODEL->currentControlledPkg(),MODEL->currentControlledUser().uid);
        startCheckCurrentScreen();
    }else {
        LOG << "fbid is empty";
    }
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

void MainController::executeRequiredActions()
{
    LOG << "Current package: " << MODEL->currentControlledPkg();
    if(MODEL->currentControlledUser()._id == ""){
        MODEL->currentControlledUser() = WEB_API->cloneUserData();
    }else{
        LOG << "User info has storaged already";
    }

    MODEL->clearActionList();
    WEB_API->getDoAction();
}

void MainController::doAction()
{
    LOG << "Current action: " << MODEL->currentAction();
    if(MODEL->currentAction()["action"] == "viplike"){
        this->execVipLike();
    }else{
        // Do them later
    }
}
