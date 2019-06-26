#ifndef MODEL_H
#define MODEL_H

#include <QGuiApplication>
#include <QObject>
#include <QTimer>
#include <QDebug>
#include <QScreen>
#include <QQuickView>
#include <QTest>
#include <QString>
#include "AppDefines.h"
#include <QThread>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

class Model : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString token READ token WRITE setToken NOTIFY tokenChanged)

public:
    static Model* instance();

    Q_INVOKABLE void start();

public:
    QString token() const;
    void setToken(QString data);

    DEVICE_INFO deviceInfo() const;
    void setDeviceInfo(DEVICE_INFO data);

    APP_CONFIG appConfig() const;
    void setAppConfig(APP_CONFIG data);

    USER_DATA userData() const;
    void setUserData(USER_DATA data);

    void updateUserData(QString packageName, USER_DATA data);

    QMap<QString, USER_DATA>& getUserDataList();

    QString currentControlledPkg();
    USER_DATA &currentControlledUser();
    void nextCurrentControlledObj();

    void loadUserDataList();
    void saveUserDataList();

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

signals:
    void sigStartProgram();
    void tokenChanged();
    void nextCurrentControlledObjChanged();

public slots:
};

#endif // MODEL_H
