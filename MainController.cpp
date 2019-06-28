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
    connect(MODEL,SIGNAL(currentActionListDone()),this,SLOT(updateResult()));
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
    LOG << "currentScreen: " << currentScreen();
    switch(currentScreen()){
    case AppEnums::HMI_START_UP_SCREEN:
        // Do nothing
        break;
    case AppEnums::HMI_LOGIN_SCREEN:
        break;
    case AppEnums::HMI_TURNON_FIND_FRIEND_SCREEN:
        ShellOperation::findAndClick(SKIP_FIND_FRIEND_BTN);
        break;
    case AppEnums::HMI_SAVE_LOGIN_INFO_SCREEN:
        ShellOperation::findAndClick(OK_BTN_FOR_SAVE_LOGIN);
        break;
    case AppEnums::HMI_NEW_FEED_SCREEN:
        if(MODEL->currentAction()["action"].toString() == "viplike"){
            if(MODEL->currentAction()["fbid"].toString() != ""){
                JAVA_COM->openFBLiteWithUserID(MODEL->currentControlledPkg(),MODEL->currentAction()["fbid"].toString());

                for(int i = 0; i < 2; i++){
                    if(!ShellOperation::findAndClick(LIKE_ICON))
                        i--;
                    ShellOperation::callScrollEvent(QPoint(500,1300),QPoint(500,600));
                    delay(200);
                }
                MODEL->nextCurrentAction();
            }else {
                LOG << "fbid is empty";
            }
        }else{
            LOG << "current action: " << MODEL->currentAction()["action"].toString();
        }
        break;
    default:
        break;
    }
}

void MainController::onChangeAcitivity()
{
    LOG;
    if(currentActivity() == FACEBOOK_LITE_SCREEN){
        this->startCheckCurrentScreen();
        delay(1000);
    }else{
#ifdef ANDROID_KIT
//        JavaCommunication::instance()->openFBLiteApplication(FACEBOOK_LITE_PKGNAME,FACEBOOK_LITE_ACT);
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
    LOG << " -------------------New Action -----------------";
    LOG << "Current action: " << MODEL->currentAction();
    if(MODEL->currentAction()["action"].toString() == "viplike"){
        if(MODEL->currentAction()["fbid"].toString() != ""){
            this->setCurrentScreen(AppEnums::HMI_UNKNOW_SCREEN);
            JAVA_COM->openFBLiteWithUserID(MODEL->currentControlledPkg(),"");
            startCheckCurrentScreen();
        }else {
            LOG << "fbid is empty";
        }
    }else{
        LOG << "There is no action";
    }
}

void MainController::updateResult()
{
    WEB_API->getDoResult();
}
