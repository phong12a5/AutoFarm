#include "CheckCurrSrcThread.hpp"
#include "MainController.hpp"

#define MAIN_CTRL   MainController::instance()

CheckCurrSrcThread::CheckCurrSrcThread(QObject *parent) : QObject(parent)
{
    // Do nothing
}

CheckCurrSrcThread::~CheckCurrSrcThread()
{
    LOG;
    m_updateCurrSrcTimer->stop();
    delete m_updateCurrSrcTimer;
}

bool CheckCurrSrcThread::isOnScreen(QString iconPath)
{
    bool retVal = false;
#ifdef ANDROID_KIT
    QString screenImgPath = ShellOperation::screenShot();
    QPoint point = ImageProcessing::findImageOnImage(iconPath,screenImgPath);
    if(!point.isNull()){
        retVal = true;
    }else{
        retVal = false;
    }
#else
    Q_UNUSED(iconPath);
    return true;
#endif
    return retVal;
}

bool CheckCurrSrcThread::isCurrentScreen(int screenID)
{
    bool retVal = false;
    switch (screenID) {
        case AppEnums::HMI_START_UP_SCREEN:
        retVal = isOnScreen(LOADING_SCREEN2);
        break;
    case AppEnums::HMI_LOGIN_SCREEN:
        retVal = isOnScreen(LOGIN_SCREEN_1);
        break;
    case AppEnums::HMI_INCORRECT_PASSWORD_SCREEN:
        retVal = isOnScreen(INCORRECT_PASSWORD);
        break;
    case AppEnums::HMI_MISSING_PASSWORD_SCREEN:
        retVal = isOnScreen(MISSING_PWD_SCREEN);
        break;
    case AppEnums::HMI_CONFIRM_INDENTIFY_SCREEN:
        retVal = isOnScreen(CONFIRM_IDENTIFY) || isOnScreen(CONFIRM_IDENTIFY2);
        break;
    case AppEnums::HMI_DEACTIVE_ACCOUNT_SCREEN:
        retVal = isOnScreen(DOWNLOAD_INFO_BTN);
        break;
    case AppEnums::HMI_TURNON_FIND_FRIEND_SCREEN:
        retVal = isOnScreen(TURNON_FIND_FRIEND);
        break;
    case AppEnums::HMI_SAVE_LOGIN_INFO_SCREEN:
        retVal = isOnScreen(SAVE_LOGIN_SCREEN);
        break;
    case AppEnums::HMI_CHOOSE_AVATAR_SCREEN:
        retVal = isOnScreen(CHOOSE_AVATAR);
        break;
    case AppEnums::HMI_ADDFRIEND_SUGGESTION_SCREEN:
        retVal = isOnScreen(ADD_FRIEND_SUGGESTION2);
        break;
    case AppEnums::HMI_NEW_FEED_SCREEN:
        retVal = isOnScreen(NEWSFEED_ICON) || isOnScreen(NEWSFEED_ICON2);
        break;
    }
//    LOG << "screenID: " << MAIN_CTRL->screenStr(screenID) << " : " << retVal;
    return retVal;
}

int CheckCurrSrcThread::findScreen() const
{
    LOG;
    int retVal = 0;
    return retVal;
}

void CheckCurrSrcThread::doWork(const QString &parameter)
{
    Q_UNUSED(parameter);
    m_updateCurrSrcTimer = new QTimer(this);
    m_updateCurrSrcTimer->setInterval(2000);
    m_updateCurrSrcTimer->setSingleShot(false);
    QObject::connect(m_updateCurrSrcTimer, SIGNAL(timeout()), this, SLOT(onUpdateCurrentScreen()));
    m_updateCurrSrcTimer->start();
}

void CheckCurrSrcThread::onUpdateCurrentScreen()
{
    if(m_updateCurrSrcTimer->interval() == 2000)
        m_updateCurrSrcTimer->setInterval(1000);

    QList<int> screenPiorityOrder;
    int currentScreen = MAIN_CTRL->currentScreen();
    screenPiorityOrder <<   AppEnums::HMI_START_UP_SCREEN
                       <<   AppEnums::HMI_NEW_FEED_SCREEN
                       <<   AppEnums::HMI_LOGIN_SCREEN
                       <<   AppEnums::HMI_CONFIRM_INDENTIFY_SCREEN
                       <<   AppEnums::HMI_MISSING_PASSWORD_SCREEN
                       <<   AppEnums::HMI_INCORRECT_PASSWORD_SCREEN
                       <<   AppEnums::HMI_DEACTIVE_ACCOUNT_SCREEN
                       <<   AppEnums::HMI_SAVE_LOGIN_INFO_SCREEN
                       <<   AppEnums::HMI_CHOOSE_AVATAR_SCREEN
                       <<   AppEnums::HMI_TURNON_FIND_FRIEND_SCREEN
                       <<   AppEnums::HMI_ADDFRIEND_SUGGESTION_SCREEN;

    QList<int> checkedScreenList = screenPiorityOrder;
    if(screenPiorityOrder.contains(currentScreen)){
        while (checkedScreenList.indexOf(currentScreen) != 0) {
            checkedScreenList.append(checkedScreenList.takeFirst());
        }
    }

    foreach (int i , checkedScreenList){
        LOG << "Checking: " << MAIN_CTRL->screenStr(i);

        if(MAIN_CTRL->currentScreen() != currentScreen){
            LOG << "Current is changed from another thread";
            break;
        }

        if(isCurrentScreen(i)){
            MAIN_CTRL->setCurrentScreen(i);
            delay(2000);
            break;
        }
    }
}
