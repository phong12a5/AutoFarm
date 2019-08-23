#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

#include <QObject>
#include <QJsonObject>

#include "AppDefines.hpp"
#include "AppEnums.hpp"
#include "FarmActions.hpp"
#include "AutoFarmerAPIsWraper.hpp"
#include "AutoFarmerDefine.hpp"
#include "ScreenAnalysis.h"

class MainController : public QObject
{
    Q_OBJECT

public:
    explicit MainController(QObject *parent = nullptr);
    ~MainController();
    void initController();

private:
    void loadUserDataList();
    void saveUserDataList();
    QJsonDocument loadJson(QString fileName);
    void saveJson(QJsonDocument document, QString fileName);
    void downloadAndInstallPackages();
#if 0
    void startCheckCurrentScreen();
#endif

private:
    QTimer m_changeScreenTimer;
    AutoFarmerAPIsWraper m_famerAPIs;
    FarmActions* m_farmAction;
    ScreenAnalysis* m_screenAnalysis;
#if 0
    QThread                 m_checkScreenThread;
    CheckCurrSrcWorker      checkScreenWorker;
#endif
signals:
#if 0
    void sigStartCheckCurrentScreen();
#endif

public slots:
    void onStartProgram();
    void onChangeScreen();
    void executeRequiredActions();
    void updateResult();
    void onFinishedListObject();
    void onchangeScreenTimerTimeout();
    void onUpdateCurrentScreen(int screenID);
};

#endif // MAINCONTROLLER_H
