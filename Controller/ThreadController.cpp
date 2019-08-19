#include "ThreadController.hpp"
#include "AppMain.hpp"

ThreadController::ThreadController(QObject *parent) : QObject(parent)
{

}

ThreadController::~ThreadController()
{
    m_checkScreenThread.quit();
    m_startNewActivityThread.quit();

    m_checkScreenThread.wait();
    m_startNewActivityThread.wait();
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
    connect(&checkScreenWorker, &CheckCurrSrcThread::screenChanged,MainController::instance(), &MainController::onChangeScreen);
    m_checkScreenThread.start();
    emit sigStartCheckCurrentScreen("Phong DT");
}

