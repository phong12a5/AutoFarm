#include "CheckCurrActivityThread.hpp"
#include "AppMain.hpp"

#define MAIN_CONTROLLER MainController::instance()

CheckCurrActivityThread::CheckCurrActivityThread(QObject *parent) : QObject(parent)
{
    // Do nothing
}

CheckCurrActivityThread::~CheckCurrActivityThread()
{
    LOG;
    m_updateCurrActTimer->stop();
    delete m_updateCurrActTimer;
}

void CheckCurrActivityThread::doWork(const QString &parameter)
{
    LOG << "[CheckCurrSrcThread] " << parameter;
    m_updateCurrActTimer = new QTimer(this);
    m_updateCurrActTimer->setInterval(1000);
    m_updateCurrActTimer->setSingleShot(false);
    QObject::connect(m_updateCurrActTimer, SIGNAL(timeout()), this, SLOT(onUpdateCurrentActivity()));
    m_updateCurrActTimer->start();    
}

void CheckCurrActivityThread::onUpdateCurrentActivity()
{
#ifdef ANDROID_KIT
    QString currentAct = ShellOperation::getCurrentActivity();
    if(currentAct != ""){
        QMutex mutex;
        mutex.lock();
        MAIN_CONTROLLER->setCurrentActivity(currentAct);
        mutex.unlock();
    }else{
        LOG << "Invalid activity";
    }
#endif
}
