#include "MainController.hpp"
#include "Communication/WebAPI.hpp"
#include "Model.hpp"

#define WEB_API WebAPI::instance()
#define MODEL Model::instance()
#define JAVA_COM JavaCommunication::instance()

MainController* MainController::m_instance = nullptr;

MainController::MainController(QObject *parent) : QObject(parent)
{
    m_currentScreen = -1;
    m_currentActivity = "";
}

void MainController::execVipLike()
{
    LOG;
    switch(currentScreen()){
    case AppEnums::HMI_START_UP_SCREEN:
        // Do nothing
        break;
    case AppEnums::HMI_LOGIN_SCREEN:
    {
#ifdef ANDROID_KIT
        QPoint tmp_point = ShellOperation::findAnImageOnScreen(LOGIN_BTN);
        if(tmp_point.isNull()){
            LOG << "Couldn't find Login button";
            ShellOperation::findAndClick(ENGLISH_BTN);
            delay(100);
        }
        ShellOperation::findAndClick(EMAIL_FIELD);
        ShellOperation::enterText(MODEL->currentControlledUser().email);
        ShellOperation::findAndClick(PASSWORD_FIELD);
        ShellOperation::enterText(MODEL->currentControlledUser().password);
        delay(2000);
        ShellOperation::findAndClick(LOGIN_BTN);
#endif
    }
        break;
    case AppEnums::HMI_INCORRECT_PASSWORD_SCREEN:
        MODEL->currentControlledUser() = WEB_API->cloneUserData();
        MODEL->saveUserDataList();
        ShellOperation::clearPackageData(MODEL->currentControlledPkg());
        emit MODEL->nextCurrentControlledObjChanged();
        break;
    case AppEnums::HMI_CONFIRM_INDENTIFY_SCREEN:
        WEB_API->updateCheckPoint();
        MODEL->currentControlledUser() = WEB_API->cloneUserData();
        MODEL->saveUserDataList();
        ShellOperation::clearPackageData(MODEL->currentControlledPkg());
        emit MODEL->nextCurrentControlledObjChanged();
        break;
    case AppEnums::HMI_TURNON_FIND_FRIEND_SCREEN:
#ifdef ANDROID_KIT
        ShellOperation::findAndClick(SKIP_FIND_FRIEND_BTN);
#endif
        break;
    case AppEnums::HMI_SAVE_LOGIN_INFO_SCREEN:
#ifdef ANDROID_KIT
        ShellOperation::findAndClick(OK_BTN_FOR_SAVE_LOGIN);
#endif
        break;
    case AppEnums::HMI_NEW_FEED_SCREEN:
#ifdef ANDROID_KIT
        if(MODEL->currentAction()["action"].toString() == "viplike"){
            if(MODEL->currentAction()["fbid"].toString() != ""){
                JAVA_COM->openFBLiteWithUserID(MODEL->currentControlledPkg(),MODEL->currentAction()["fbid"].toString());

                int count = 0;
                while(count < NUMBER_CLICK){
                    if(ShellOperation::findAndClick(LIKE_ICON)) count ++;
                    if(count < NUMBER_CLICK)ShellOperation::callScrollEvent(QPoint(540,1770),QPoint(540,230));
                    delay(200);
                }
                MODEL->nextCurrentAction();
            }else {
                LOG << "fbid is empty";
            }
        }else{
            LOG << "current action: " << MODEL->currentAction()["action"].toString();
        }
#endif
        break;
    default:
        break;
    }
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
    connect(MODEL,SIGNAL(finishedListObject()),this,SLOT(onFinishedListObject()));
}

void MainController::startLoop()
{
    LOG;
    WEB_API->getApk();
    WEB_API->installAllPackages();
    QEventLoop evenLoop;
    connect(WEB_API, SIGNAL(installAllPackagesCompleted()), &evenLoop, SLOT(quit()));
    evenLoop.exec();
    MODEL->nextCurrentControlledObj();
}

int MainController::currentScreen() const
{
    return m_currentScreen;
}

QString MainController::currentScreenStr() const
{
    QString retVal = "";
    switch (m_currentScreen) {
    case AppEnums::HMI_UNKNOW_SCREEN:
        retVal = "HMI_UNKNOW_SCREEN";
        break;
    case AppEnums::HMI_START_UP_SCREEN:
        retVal = "HMI_START_UP_SCREEN";
        break;
    case AppEnums::HMI_LOGIN_SCREEN:
        retVal = "HMI_LOGIN_SCREEN";
        break;
    case AppEnums::HMI_INCORRECT_PASSWORD_SCREEN:
        retVal = "HMI_INCORRECT_PASSWORD_SCREEN";
        break;
    case AppEnums::HMI_CONFIRM_INDENTIFY_SCREEN:
        retVal = "HMI_CONFIRM_INDENTIFY_SCREEN";
        break;
    case AppEnums::HMI_TURNON_FIND_FRIEND_SCREEN:
        retVal = "HMI_TURNON_FIND_FRIEND_SCREEN";
        break;
    case AppEnums::HMI_SAVE_LOGIN_INFO_SCREEN:
        retVal = "HMI_SAVE_LOGIN_INFO_SCREEN";
        break;
    case AppEnums::HMI_NEW_FEED_SCREEN:
        retVal = "HMI_NEW_FEED_SCREEN";
        break;
    default:
        break;
    }
    return retVal;
}

void MainController::setCurrentScreen(const int data)
{
    if(m_currentScreen != data){
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
    LOG << "currentScreen: " << currentScreenStr();
    if(MODEL->currentAction()["action"].toString() == "viplike"){
        this->execVipLike();
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
        MODEL->saveUserDataList();
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
#ifdef ANDROID_KIT
            JAVA_COM->openFBLiteWithUserID(MODEL->currentControlledPkg(),"");
#endif
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
#ifdef ANDROID_KIT
    ShellOperation::killSpecificApp(MODEL->currentControlledPkg());
#endif
}

void MainController::onFinishedListObject()
{
    LOG;
    if(MODEL->deviceInfo().isNox == "true"){
        QProcess process;
        process.start(QString("touch %1%2").arg(ENDSCRIPT_PATH).arg(ENDSCRIPT_FILENAME));
        process.waitForFinished(-1);
    }
}
