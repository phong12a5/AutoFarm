#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

#include <QObject>
#include "AppDefines.hpp"
#include "Communication/JavaCommunication.hpp"
#include "Processing/ImageProcessing.hpp"
#include "Controller/ThreadController.hpp"
#include "AppEnums.hpp"
#include "Communication/WebAPI.hpp"

class MainController : public QObject
{
    Q_OBJECT

private:
    explicit MainController(QObject *parent = nullptr);

private:
    void execVipLike();
    void installPackages(QStringList,QStringList);

public:
    static MainController* instance();
    void initController();
    void downloadAndInstallPackages();

public:
    int currentScreen() const;
    QString screenStr(int screenID) const;
    void setCurrentScreen(const int data);

    QString currentActivity() const;
    void setCurrentActivity(const QString data);

    void startCheckCurrentScreen();
    void startCheckCurrentActivity();

private:
    static MainController* m_instance;

    ThreadController multiThreadController;

    int m_currentScreen;
    QString m_currentActivity;
    QTimer m_changeScreenTimer;

signals:
    void currentScreenChanged();
    void currentActivityChanged();

public slots:
    void onChangeScreen();
    void onChangeAcitivity();
    void executeRequiredActions();
    void doAction();
    void updateResult();
    void onFinishedListObject();
    void onDownloadCompleted(QStringList,QStringList);
    void onchangeScreenTimerTimeout();
};

#endif // MAINCONTROLLER_H
