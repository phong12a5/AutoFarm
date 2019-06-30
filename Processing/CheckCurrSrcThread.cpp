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
#ifdef ANDROID_KIT
    QString screenImgPath = ShellOperation::screenShot();
    QPoint point = ImageProcessing::findImageOnImage(iconPath,screenImgPath,0.99);
    if(!point.isNull()){
        return true;
    }else{
        return false;
    }
#else
    Q_UNUSED(iconPath);
    return true;
#endif
}

bool CheckCurrSrcThread::isCurrentScreen(int screenID)
{
    bool retVal = false;
    switch (screenID) {
        case AppEnums::HMI_START_UP_SCREEN:
        retVal = isOnScreen(LOADING_ICON);
        break;
    case AppEnums::HMI_LOGIN_SCREEN:
        retVal = isOnScreen(LOGIN_SCREEN);
        break;
    case AppEnums::HMI_INCORRECT_PASSWORD_SCREEN:
        retVal = isOnScreen(INCORRECT_PASSWORD);
        break;
    case AppEnums::HMI_CONFIRM_INDENTIFY_SCREEN:
        retVal = isOnScreen(CONFIRM_IDENTIFY);
        break;
    case AppEnums::HMI_TURNON_FIND_FRIEND_SCREEN:
        retVal = isOnScreen(TURNON_FIND_FRIEND);
        break;
    case AppEnums::HMI_SAVE_LOGIN_INFO_SCREEN:
        retVal = isOnScreen(SAVE_LOGIN_SCREEN);
        break;
    case AppEnums::HMI_NEW_FEED_SCREEN:
        retVal = isOnScreen(NEWSFEED_ICON);
        break;
    }

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
    LOG << "[CheckCurrSrcThread] " << parameter;
    m_updateCurrSrcTimer = new QTimer(this);
    m_updateCurrSrcTimer->setInterval(100);
    m_updateCurrSrcTimer->setSingleShot(false);
    QObject::connect(m_updateCurrSrcTimer, SIGNAL(timeout()), this, SLOT(onUpdateCurrentScreen()));
    m_updateCurrSrcTimer->start();

}

void CheckCurrSrcThread::onUpdateCurrentScreen()
{
    QList<int> screenPiorityOrder;
    screenPiorityOrder << AppEnums::HMI_NEW_FEED_SCREEN
                       << AppEnums::HMI_LOGIN_SCREEN
                       << AppEnums::HMI_INCORRECT_PASSWORD_SCREEN
                       << AppEnums::HMI_CONFIRM_INDENTIFY_SCREEN
                       << AppEnums::HMI_TURNON_FIND_FRIEND_SCREEN
                       << AppEnums::HMI_SAVE_LOGIN_INFO_SCREEN
                       << AppEnums::HMI_START_UP_SCREEN;

    QList<int> checkedScreenList = screenPiorityOrder;

    if(screenPiorityOrder.contains(MAIN_CTRL->currentScreen())){
        while (checkedScreenList.indexOf(MAIN_CTRL->currentScreen()) != 0) {
            checkedScreenList.append(checkedScreenList.takeFirst());
        }
    }

    for(int i = 0; i < checkedScreenList.length(); i++){
        if(isCurrentScreen(checkedScreenList.at(i))){
            MAIN_CTRL->setCurrentScreen(checkedScreenList.at(i));
            break;
        }
    }
}
