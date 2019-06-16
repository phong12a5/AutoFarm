#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

#include <QObject>
#include "AppDefines.h"
#include "Communication/JavaCommunication.h"
#include "Processing/ImageProcessing.h"
#include "ThreadController.h"
#include "AppEnums.h"

class MainController : public QObject
{
    Q_OBJECT

private:
    explicit MainController(QObject *parent = nullptr);

public:
    static MainController* instance();
    void initController();

public:
    int currentScreen() const;
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
signals:
    void currentScreenChanged();
    void currentActivityChanged();

public slots:
    void onChangeScreen();
    void onChangeAcitivity();

};

#endif // MAINCONTROLLER_H
