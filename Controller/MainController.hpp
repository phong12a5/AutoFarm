#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

#include <QObject>
#include <QJsonObject>

#include "AppDefines.hpp"
#include "AppEnums.hpp"
#include "FarmActions.hpp"
#include "AutoFarmerAPIsWraper.hpp"
#include "AutoFarmerDefine.hpp"
#include "CheckCurrSrcWorker.hpp"

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
    void startCheckCurrentScreen();


private:
    QTimer m_changeScreenTimer;
    AutoFarmerAPIsWraper m_famerAPIs;
    FarmActions* m_farmAction;
    QThread                 m_checkScreenThread;
    CheckCurrSrcWorker      checkScreenWorker;

signals:
    void sigStartCheckCurrentScreen();

public slots:
    void onStartProgram();
    void onChangeScreen(int screenID);
    void executeRequiredActions();
    void updateResult();
    void onFinishedListObject();
    void onchangeScreenTimerTimeout();
};

#endif // MAINCONTROLLER_H
