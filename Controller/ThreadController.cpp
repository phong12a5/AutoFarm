#include "ThreadController.hpp"
#include "AppMain.hpp"

ThreadController::ThreadController(QObject *parent) : QObject(parent)
{

}

ThreadController::~ThreadController()
{
    m_checkScreenThread.quit();
    m_checkActivityThread.quit();

    m_checkScreenThread.wait();
    m_checkActivityThread.wait();
}

void ThreadController::startCheckCurrentScreen()
{
    LOG << "[ThreadController]";
    if(m_checkScreenThread.isRunning()){
        LOG << "m_checkScreenThread is running already";
        return;
    }
    checkScreenWorker.moveToThread(&m_checkScreenThread);
    connect(&m_checkScreenThread, &QThread::finished, &checkScreenWorker, &QObject::deleteLater);
    connect(this, &ThreadController::sigStartCheckCurrentScreen, &checkScreenWorker, &CheckCurrSrcThread::doWork);
    m_checkScreenThread.start();
    emit sigStartCheckCurrentScreen("Phong DT");
}

void ThreadController::startCheckCurrentActivity()
{
    LOG << "[ThreadController]";
    if(m_checkActivityThread.isRunning()){
        LOG << "m_checkActivityThread is running already";
        return;
    }
    checkActivityWorker.moveToThread(&m_checkActivityThread);
    connect(&m_checkActivityThread, &QThread::finished, &checkActivityWorker, &QObject::deleteLater);
    connect(this, &ThreadController::sigStartCheckCurrentActivity, &checkActivityWorker, &CheckCurrActivityThread::doWork);
    m_checkActivityThread.start();
    emit sigStartCheckCurrentActivity(QString("Phong DT"));
}

