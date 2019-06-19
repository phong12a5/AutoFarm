#include "CheckCurrSrcThread.h"
#include "Controller/MainController.h"

#define MAIN_CTRL    MainController::instance()

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
    Q_UNUSED(iconPath);
//    QString screenImgPath = ShellOperation::screenShot();
//    QPoint point = ImageProcessing::findImageOnImage(QDir::currentPath() + iconPath,screenImgPath);
//    if(!point.isNull()){
//        return true;
//    }else{
//        screenImgPath = ADBCommand::screenShot();
//        point = ImageProcessing::findImageOnImage(QDir::currentPath() + iconPath,screenImgPath);
//        if(!point.isNull()){
//            return true;
//        }else{
//            return false;
//        }
//    }
    return false;
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

    LOG << "[RegFBController]" << "screenID: " << screenID << " --- reval:" << retVal;

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
    m_updateCurrSrcTimer->setInterval(500);
    m_updateCurrSrcTimer->setSingleShot(false);
    QObject::connect(m_updateCurrSrcTimer, SIGNAL(timeout()), this, SLOT(onUpdateCurrentScreen()));
    m_updateCurrSrcTimer->start();

}

void CheckCurrSrcThread::onUpdateCurrentScreen()
{
//    int _tmpScr = MAIN_CTRL->currentScreen();
//    for
}
