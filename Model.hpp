#ifndef MODEL_HPP
#define MODEL_HPP

#include <QGuiApplication>
#include <QObject>
#include <QTimer>
#include <QDebug>
#include <QScreen>
#include <QQuickView>
#include <QTest>
#include <QString>
#include "AppDefines.hpp"
#include <QThread>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

class Model : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString token READ token WRITE setToken NOTIFY tokenChanged)
    Q_PROPERTY(QString deviceType READ deviceType CONSTANT)
    Q_PROPERTY(QString logContent READ logContent WRITE setLogContent NOTIFY logContentChanged)
    Q_PROPERTY(bool autoStart READ autoStart WRITE setAutoStart NOTIFY autoStartChanged)

public:
    static Model* instance();

    Q_INVOKABLE void start();

public:
    QString token() const;
    void setToken(QString data);

    bool autoStart() const;
    void setAutoStart(bool data);

    QString deviceType() const;

    bool isNoxDevice() const;

    DEVICE_INFO deviceInfo() const;
    void setDeviceInfo(DEVICE_INFO data);

    QString logContent();
    void setLogContent(QString data);

    APP_CONFIG appConfig() const;
    void setAppConfig(APP_CONFIG data);

    QMap<QString, USER_DATA>* getUserDataList();
    QString currentControlledPkg();
    USER_DATA currentControlledUser();
    void updateCurrentControlleredUser(USER_DATA data);
    void nextCurrentControlledObj();

    QList<QJsonObject> actionList();
    void setActionList(QList<QJsonObject> data);
    void clearActionList();

    void loadUserDataList();
    void saveUserDataList();

    void loadAppConfig();
    void saveAppConfig();

    int currentScreen() const;
    QString screenStr(int screenID) const;
    void setCurrentScreen(const int data);

private:
    explicit Model(QObject *parent = nullptr);

    QJsonDocument loadJson(QString fileName);
    void saveJson(QJsonDocument document, QString fileName);

private:

private:
    static Model* m_instance;

    QString m_token;
    DEVICE_INFO m_deviceInfo;
    APP_CONFIG m_appConfig;
    USER_DATA m_userData;
    QMap<QString, USER_DATA> m_userDataList;
    int m_currentPkgIndex;
    QList<QJsonObject> m_actionList;
    QList<QJsonObject> m_changedActionList;
    QString m_logContent;
    QString m_testingImageSource;
    QString m_screenName;
    bool m_autoStart;
    int m_currentScreen;

signals:
    void sigStartProgram();
    void tokenChanged();
    void nextCurrentControlledObjChanged();
    void currentActionListDone();
    void finishedListObject();
    void logContentChanged();
    void testingImageSourceChanged();
    void screenNameChanged();
    void autoStartChanged();
    void currentScreenChanged();

public slots:
};

#endif // MODEL_H
