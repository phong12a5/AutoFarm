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

private:
    explicit Model(QObject *parent = nullptr);

private:

private:
    static Model* m_instance;

    QString m_token;
    DEVICE_INFO m_deviceInfo;
signals:
    void sigStartProgram();
    void tokenChanged();

public slots:
};

#endif // MODEL_H
