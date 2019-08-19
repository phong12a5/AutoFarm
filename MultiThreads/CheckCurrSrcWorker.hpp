#ifndef CHECKCURRSRCTHREAD_H
#define CHECKCURRSRCTHREAD_H

#include <QObject>
#include <AppDefines.hpp>
#include <QTimer>
#include <QMutex>
#include "AppEnums.hpp"
#include "ImageProcessing.hpp"
#include "AutoFarmerAPIsWraper.hpp"

class CheckCurrSrcWorker : public QObject
{
    Q_OBJECT
public:
    explicit CheckCurrSrcWorker(QObject *parent = nullptr);
    ~CheckCurrSrcWorker();
    void setAutoFarmerAPIs(AutoFarmerAPIsWraper autoFamerAPIs);

private:
    bool isOnScreen(QString iconPath);
    bool isCurrentScreen(int screenID);

private:
    cv::Mat m_screenImg;
    QTimer* m_updateCurrSrcTimer;
    AutoFarmerAPIsWraper m_autoFamerAPIs;
signals:
    void screenChanged(int screenID);

public slots:
    void doWork();
    void onUpdateCurrentScreen();
};

#endif // CHECKCURRSRCTHREAD_H
