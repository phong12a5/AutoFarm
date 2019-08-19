#ifndef AUTOFARMERAPISWRAPER_H
#define AUTOFARMERAPISWRAPER_H

#include <QPoint>
#include <QJsonArray>
#include "AutoFarmerAPI.hpp"
#include "AppDefines.hpp"

class AutoFarmerAPIsWraper : public AutoFarmerAPI
{
public:
    AutoFarmerAPIsWraper();
    ~AutoFarmerAPIsWraper();


    bool                w_initEnv(const QString token, const APPNAME_ID appnameId);
    APP_CONFIG          w_getConfig();
    QJsonObject         w_getApk();
    QString             w_downloadApk(QString url);
    USER_DATA           w_getClone();
    QJsonObject         w_getCloneInfo();
    QJsonObject         w_postClone(QJsonObject data);
    QList<QJsonObject>  w_doAction(QString fbId);
    bool                w_doResult(QString fbId, QJsonObject data);
    bool                w_doClick(int x, int y);
    bool                w_swipe(QPoint from, QPoint to, int delay);
    bool                w_inputText(const QString text);
    bool                w_inputKeyEvent(const QString keyCode);
    QString             w_screenCapture(const QString fullPath);
    bool                w_installPackage(QString apkPath);
    bool                w_isExistPackage(QString packageName);
    bool                w_uninstallPackage(QString packageName);
    bool                w_openPackage(QString packageName, QString activity);
    bool                w_closePackage(QString packageName);
    bool                w_wipePackage(QStringList packageNames);
    QJsonObject         w_getTextFromImage(QString imagePath);
    QPoint              w_findImageFromImage(QString templatePath, QString imagePath);
    bool                findAndClick(QString iconPath);
    bool                enterKeyBoard();
};

#endif // AUTOFARMERAPISWRAPER_H
