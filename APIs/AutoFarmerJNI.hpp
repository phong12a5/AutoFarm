#ifndef AUTOFARMERJNI_H
#define AUTOFARMERJNI_H

#include <QObject>
#include <QFile>
#include <QAndroidJniObject>
#include <QAndroidJniEnvironment>
#include <QtAndroid>
#include "AppDefines.hpp"
#include "AutoFarmerLogger.hpp"

class AutoFarmerJNI : public QObject
{
    Q_OBJECT

private:
    explicit AutoFarmerJNI(QObject *parent = nullptr);

public:
    static AutoFarmerJNI* instance();

    void openFBLiteApplication(QString packageName, QString activityName) const;
    void openFBLiteWithUserID(QString packageName, QString userID) const;

private:
    static AutoFarmerJNI* m_instance;
signals:

public slots:
};

#endif // AUTOFARMERJNI_H
