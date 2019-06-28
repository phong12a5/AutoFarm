#include "CheckCurrSrcThread.h"
#include "MainController.h"

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
    QString screenImgPath = ShellOperation::screenShot();
    QPoint point = ImageProcessing::findImageOnImage(iconPath,screenImgPath,0.99);
    if(!point.isNull()){
        return true;
    }else{
        return false;
    }
}

bool CheckCurrSrcThread::isCurrentScreen(int screenID) const
{
    bool retVal = false;
//    switch (screenID) {
//        case AppEnums::E_FBLITE_SCREEN_ID_LOGIN:
//        retVal = ADBCommand::isOnScreen(CREATE_NEW_FBACC_ICON);
//        break;
//    case AppEnums::E_FBLITE_SCREEN_ID_JOIN_FB:
//        retVal = ADBCommand::isOnScreen(JOIN_FB_TEXT);
//        break;
//    }

//    LOG << "[RegFBController]" << "screenID: " << screenID << " --- reval:" << retVal;

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
    if(isOnScreen(LOADING_ICON)){
        MAIN_CTRL->setCurrentScreen(AppEnums::HMI_START_UP_SCREEN);
    }else if(isOnScreen(NEWSFEED_ICON)){
        MAIN_CTRL->setCurrentScreen(AppEnums::HMI_NEW_FEED_SCREEN);
    }else {
        MAIN_CTRL->setCurrentScreen(AppEnums::HMI_UNKNOW_SCREEN);
    }
}
