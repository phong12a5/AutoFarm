#include "CheckCurrSrcThread.hpp"
#include "MainController.hpp"
#include "Model.hpp"

#define MODEL       Model::instance()

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
    QPoint point = ImageProcessing::findImageOnImage(iconPath,m_screenImg);
    if(!point.isNull()){
        retVal = true;
    }else{
        retVal = false;
    }
#else
    Q_UNUSED(iconPath);
    return true;
#endif
    LOG << iconPath.split("/").last() << " : " << retVal;
    return retVal;
}

bool CheckCurrSrcThread::isCurrentScreen(int screenID)
{
    bool retVal = false;
    switch (screenID) {
    case AppEnums::HMI_SELECT_LANGUAGE_SCREEN:
        retVal = isOnScreen(SELECT_LANGUAGE_SCREEN);
        break;
    case AppEnums::HMI_LOGIN_SCREEN:
        retVal = isOnScreen(LOGIN_SCREEN);
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
    case AppEnums::HMI_LOGIN_AGAIN_SCREEN:
        retVal = isOnScreen(LOGIN_AGAIN_ICON);
        break;
    }
    LOG << "Checking " << MODEL->screenStr(screenID) << " : " << retVal;
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
    int currentScreen = MODEL->currentScreen();
    LOG << "currentScreen: " << MODEL->screenStr(currentScreen);
    QList<int> screenPiorityOrder;
    screenPiorityOrder <<   AppEnums::HMI_NEW_FEED_SCREEN
                       <<   AppEnums::HMI_SELECT_LANGUAGE_SCREEN
                       <<   AppEnums::HMI_LOGIN_SCREEN
                       <<   AppEnums::HMI_SAVE_LOGIN_INFO_SCREEN
                       <<   AppEnums::HMI_CHOOSE_AVATAR_SCREEN
                       <<   AppEnums::HMI_TURNON_FIND_FRIEND_SCREEN
                       <<   AppEnums::HMI_ADDFRIEND_SUGGESTION_SCREEN
                       <<   AppEnums::HMI_LOGIN_AGAIN_SCREEN;


    QList<int> checkedScreenList = screenPiorityOrder;
    if(screenPiorityOrder.contains(currentScreen)){
        while (checkedScreenList.indexOf(currentScreen) != 0) {
            checkedScreenList.append(checkedScreenList.takeFirst());
        }
    }

     ShellOperation::screenshotImg(m_screenImg,"screen_checking.png");

    foreach (int i , checkedScreenList){
        if(MODEL->currentScreen() != currentScreen){
            LOG << "Current is changed from another thread";
            break;
        }

        if(i == AppEnums::HMI_LOGIN_SCREEN && currentScreen == AppEnums::HMI_LOGIN_SCREEN){
            if(isCurrentScreen(AppEnums::HMI_CONFIRM_INDENTIFY_SCREEN)){
                MODEL->setCurrentScreen(AppEnums::HMI_CONFIRM_INDENTIFY_SCREEN);
                delay(2000);
                break;
            }
            else if(isCurrentScreen(AppEnums::HMI_MISSING_PASSWORD_SCREEN)){
                MODEL->setCurrentScreen(AppEnums::HMI_MISSING_PASSWORD_SCREEN);
                delay(2000);
                break;
            }
            else if(isCurrentScreen(AppEnums::HMI_INCORRECT_PASSWORD_SCREEN)){
                MODEL->setCurrentScreen(AppEnums::HMI_INCORRECT_PASSWORD_SCREEN);
                delay(2000);
                break;
            }
            else if(isCurrentScreen(AppEnums::HMI_DEACTIVE_ACCOUNT_SCREEN)){
                MODEL->setCurrentScreen(AppEnums::HMI_DEACTIVE_ACCOUNT_SCREEN);
                delay(2000);
                break;
            }
        }

        if(isCurrentScreen(i)){
            MODEL->setCurrentScreen(i);
            delay(2000);
            break;
        }
    }
}
