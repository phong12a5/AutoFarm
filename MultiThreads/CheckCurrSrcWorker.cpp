#include "CheckCurrSrcWorker.hpp"

CheckCurrSrcWorker::CheckCurrSrcWorker(QObject *parent) : QObject(parent)
{
    // Do nothing
}

CheckCurrSrcWorker::~CheckCurrSrcWorker()
{
    LOG_DEBUG;
    m_updateCurrSrcTimer->stop();
    delete m_updateCurrSrcTimer;
}

bool CheckCurrSrcWorker::isOnScreen(QString iconPath)
{
    bool retVal = false;
    QPoint point = m_autoFamerAPIs.w_findImageFromImage(iconPath,CHECKING_SCREEN_IMG);
    if(!point.isNull()){
        retVal = true;
    }else{
        retVal = false;
    }
    LOG_DEBUG << iconPath.split("/").last() << " : " << retVal;
    return retVal;
}

bool CheckCurrSrcWorker::isCurrentScreen(int screenID)
{
    bool retVal = false;
    switch (screenID) {
    case AppEnums::HMI_SELECT_LANGUAGE_SCREEN:
        retVal = isOnScreen(SELECT_LANGUAGE_SCREEN);
        break;
    case AppEnums::HMI_LOGIN_SCREEN:
        retVal = isOnScreen(EMAIL_FIELD) && isOnScreen(PASSWORD_FIELD);
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
    case AppEnums::HMI_ACCOUNT_NOT_FOUND_SCREEN:
        retVal = isOnScreen(ACC_NOT_FOUND);
        break;
    }
    return retVal;
}

void CheckCurrSrcWorker::setAutoFarmerAPIs(AutoFarmerAPIsWraper autoFamerAPIs)
{
    m_autoFamerAPIs = autoFamerAPIs;
}

void CheckCurrSrcWorker::doWork()
{
    m_updateCurrSrcTimer = new QTimer(this);
    m_updateCurrSrcTimer->setInterval(2000);
    m_updateCurrSrcTimer->setSingleShot(false);
    QObject::connect(m_updateCurrSrcTimer, SIGNAL(timeout()), this, SLOT(onUpdateCurrentScreen()));
    m_updateCurrSrcTimer->start();
}

void CheckCurrSrcWorker::onUpdateCurrentScreen()
{
    QList<int> screenPiorityOrder;
    screenPiorityOrder <<   AppEnums::HMI_NEW_FEED_SCREEN
                       <<   AppEnums::HMI_SELECT_LANGUAGE_SCREEN
                       <<   AppEnums::HMI_LOGIN_SCREEN
                       <<   AppEnums::HMI_SAVE_LOGIN_INFO_SCREEN
                       <<   AppEnums::HMI_CHOOSE_AVATAR_SCREEN
                       <<   AppEnums::HMI_TURNON_FIND_FRIEND_SCREEN
                       <<   AppEnums::HMI_ADDFRIEND_SUGGESTION_SCREEN
                       <<   AppEnums::HMI_LOGIN_AGAIN_SCREEN;


    m_autoFamerAPIs.w_screenCapture(CHECKING_SCREEN_IMG);

    foreach (int i , screenPiorityOrder){
        if(i == AppEnums::HMI_LOGIN_SCREEN){
            if(isCurrentScreen(AppEnums::HMI_CONFIRM_INDENTIFY_SCREEN)){
                emit screenChanged(AppEnums::HMI_CONFIRM_INDENTIFY_SCREEN);
                delay(2000);
                return;
            }
            else if(isCurrentScreen(AppEnums::HMI_MISSING_PASSWORD_SCREEN)){
                emit screenChanged(AppEnums::HMI_MISSING_PASSWORD_SCREEN);
                delay(2000);
                return;
            }
            else if(isCurrentScreen(AppEnums::HMI_INCORRECT_PASSWORD_SCREEN)){
                emit screenChanged(AppEnums::HMI_INCORRECT_PASSWORD_SCREEN);
                delay(2000);
                return;
            }
            else if(isCurrentScreen(AppEnums::HMI_DEACTIVE_ACCOUNT_SCREEN)){
                emit screenChanged(AppEnums::HMI_DEACTIVE_ACCOUNT_SCREEN);
                delay(2000);
                return;
            }
            else if(isCurrentScreen(AppEnums::HMI_ACCOUNT_NOT_FOUND_SCREEN)){
                emit screenChanged(AppEnums::HMI_ACCOUNT_NOT_FOUND_SCREEN);
                delay(2000);
                return;
            }
        }

        if(isCurrentScreen(i)){
            emit screenChanged(i);
            return;
        }
    }
}
