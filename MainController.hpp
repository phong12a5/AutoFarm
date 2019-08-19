#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

#include <QObject>
#include <QJsonObject>

#include "AppDefines.hpp"
#include "Controller/ThreadController.hpp"
#include "AppEnums.hpp"
#include "Controller/FarmActions.h"
#include "AutoFarmerAPI.hpp"
#include "AutoFarmerDefine.hpp"

class MainController : public QObject
{
    Q_OBJECT

private:
    explicit MainController(QObject *parent = nullptr);

private:
    void loadUserDataList();
    void saveUserDataList();
    QJsonDocument loadJson(QString fileName);
    void saveJson(QJsonDocument document, QString fileName);
    void downloadAndInstallPackages();

public:
    static MainController* instance();
    void initController();

public:
    void startCheckCurrentScreen();

private:
    static MainController* m_instance;
    ThreadController multiThreadController;
    QTimer m_changeScreenTimer;
    AutoFarmerAPI m_farmerAPIs;

signals:

public slots:
    void onStartProgram();
    void onChangeScreen(int screenID);
    void executeRequiredActions();
    void updateResult();
    void onFinishedListObject();
    void onchangeScreenTimerTimeout();
};

#endif // MAINCONTROLLER_H
