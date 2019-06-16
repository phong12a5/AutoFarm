#ifndef JAVACOMMUNICATION_H
#define JAVACOMMUNICATION_H

#include <QObject>
#include "AppDefines.h"

#ifdef ANDROID_KIT
#include <QAndroidJniObject>
#include <QAndroidJniEnvironment>
#endif

class JavaCommunication : public QObject
{
    Q_OBJECT

private:
    explicit JavaCommunication(QObject *parent = nullptr);

public:
    static JavaCommunication* instance();

#ifdef ANDROID_KIT

    void openApplication(QString packageName, QString activityName) const;
    void backToAppMain() const;

    QString getGoogleSF();
    QString getDeviceIMEI();
    QString getDeiceIMSI();
    QString getDeviceModel();
    QString getAndroidVersion();
    QString getAndroidID();
    QString getSimCardSerialNo();
    QString getSimNumber();
    QString getWifiMacAdd();
#endif

private:
    static JavaCommunication* m_instance;
signals:

public slots:
};

#endif // JAVACOMMUNICATION_H
