#include "AutoFarmerAPIsWraper.hpp"

AutoFarmerAPIsWraper::AutoFarmerAPIsWraper()
{
    // Do thing
}

AutoFarmerAPIsWraper::~AutoFarmerAPIsWraper()
{
    // Do thing
}

bool AutoFarmerAPIsWraper::w_initEnv(const QString token, const APPNAME_ID appnameId)
{
    QJsonObject retVal = this->initEnv(token,appnameId);
    if(retVal["Status"].toBool() == false){
        LOG_DEBUG << retVal;
        return false;
    }else{
#ifdef APIS_DEBUG_MODE
        LOG_DEBUG << retVal;
#endif
        return true;
    }
}

APP_CONFIG AutoFarmerAPIsWraper::w_getConfig()
{
    APP_CONFIG config;
    QJsonObject retVal = this->getConfig();
    if(retVal["Status"].toBool() == false){
        LOG_DEBUG << retVal;
    }else{
#ifdef APIS_DEBUG_MODE
        LOG_DEBUG << retVal;
#endif
        QJsonObject responseData =  retVal["ResponseData"].toObject();
        if(!responseData.isEmpty()){
            config.timeout =    responseData["Timeout"].toInt();
            config.reset_3g =   responseData["Reset3g"].toInt();
            config.debug_mode = responseData["Debug_mode"].toString();
            config.user_type = responseData["User_type"].toInt();
            config.encryption = responseData["Encryption"].toString();
        }
    }
    return config;
}

QJsonDocument AutoFarmerAPIsWraper::w_getApk()
{
    QJsonObject retVal = this->getApk();
    if(retVal["Status"].toBool() == false){
        LOG_DEBUG << retVal;
    }else{
#ifdef APIS_DEBUG_MODE
        LOG_DEBUG << retVal;
#endif
    }
    return QJsonDocument::fromJson(retVal["ResponseData"].toString().toUtf8());
}

QString AutoFarmerAPIsWraper::w_downloadApk(QString url)
{
    QJsonObject retVal = this->downloadApk(url);
    if(retVal["Status"].toBool() == false){
        LOG_DEBUG << retVal;
    }else{
#ifdef APIS_DEBUG_MODE
        LOG_DEBUG << retVal;
#endif
    }
    return retVal["ResponseData"].toString();
}

USER_DATA AutoFarmerAPIsWraper::w_getClone()
{
    USER_DATA user_data;
    QJsonObject retVal = this->getClone();
    if(retVal["Status"].toBool() == false){
        LOG_DEBUG << retVal;
    }else{
#ifdef APIS_DEBUG_MODE
        LOG_DEBUG << retVal;
#endif
        QJsonObject jsonObj =  retVal["ResponseData"].toObject();
        if(!jsonObj.isEmpty()){
            user_data._id               = jsonObj["_id"].toString();
            user_data.uid              = jsonObj["uid"].toString();
            user_data.password         = jsonObj["password"].toString();
            user_data.cookie           = jsonObj["cookie"].toString();
            user_data.token            = jsonObj["token"].toString();
            user_data.birthday         = jsonObj["birthday"].toString();
            user_data.name             = jsonObj["name"].toString();
            user_data.sex              = jsonObj["sex"].toString();
            user_data.country          = jsonObj["country"].toString();
            user_data.email            = jsonObj["email"].toString();
            user_data.avartar          = jsonObj["avartar"].toString();
            user_data.created_date     = jsonObj["created_date"].toString();
            user_data.farming_status   = jsonObj["farming_status"].toString();
            user_data.alive_status     = jsonObj["alive_status"].toString();
            user_data.created_at       = jsonObj["created_at"].toString();
            user_data.updated_at       = jsonObj["updated_at"].toString();
            user_data.user_id          = jsonObj["user_id"].toString();
        }
    }
    return user_data;
}
/* This method has not wrapped yet */
QJsonObject AutoFarmerAPIsWraper::w_getCloneInfo()
{
    return this->getCloneInfo();
}

QJsonObject AutoFarmerAPIsWraper::w_postClone(QJsonObject data)
{
    return this->postClone(data);
}

QList<QJsonObject> AutoFarmerAPIsWraper::w_doAction(QString fbId)
{
    QList<QJsonObject> actionList;
    QJsonObject retVal = this->doAction(fbId);
    if(retVal["Status"].toBool() == false){
        LOG_DEBUG << retVal;
    }else{
#ifdef APIS_DEBUG_MODE
        LOG_DEBUG << retVal;
#endif

        QJsonObject responsedData = retVal["ResponseData"].toObject();
        if(!responsedData.isEmpty()){
            foreach (QJsonValue data, responsedData["actions"].toArray()) {
                if(data.isObject()){
                    QJsonObject action = data.toObject();
                    actionList.append(action);
                }
            }
        }
    }

    return actionList;
}

bool AutoFarmerAPIsWraper::w_doResult(QString fbId, QJsonObject data)
{
    QJsonObject retVal = this->doResult(fbId,data);
    if(retVal["Status"].toBool() == false){
        LOG_DEBUG << retVal;
        return false;
    }else{
#ifdef APIS_DEBUG_MODE
        LOG_DEBUG << retVal;
#endif
        if(retVal["ResponseData"].toString() == "success"){
            return true;
        }else{
            return false;
        }
    }
}

bool AutoFarmerAPIsWraper::w_doClick(int x, int y)
{
    LOG_DEBUG << "[" << x << "," << y << "]";
    QJsonObject retVal = this->doClick(x,y);
    if(retVal["Status"].toBool() == false){
        LOG_DEBUG << retVal;
        return false;
    }else{
#ifdef APIS_DEBUG_MODE
        LOG_DEBUG << retVal;
#endif
        return true;
    }
}

bool AutoFarmerAPIsWraper::w_swipe(QPoint from, QPoint to, int delay)
{
    QJsonObject retVal = this->swipe(from,to,delay);
    if(retVal["Status"].toBool() == false){
        LOG_DEBUG << retVal;
        return false;
    }else{
#ifdef APIS_DEBUG_MODE
        LOG_DEBUG << retVal;
#endif
        return true;
    }
}

bool AutoFarmerAPIsWraper::w_inputText(const QString text)
{
    QJsonObject retVal = this->inputText(text);
    if(retVal["Status"].toBool() == false){
        LOG_DEBUG << retVal;
        return false;
    }else{
#ifdef APIS_DEBUG_MODE
        LOG_DEBUG << retVal;
#endif
        return true;
    }
}

bool AutoFarmerAPIsWraper::w_inputKeyEvent(const QString keyCode)
{
    QJsonObject retVal = this->inputKeyEvent(keyCode);
    if(retVal["Status"].toBool() == false){
        LOG_DEBUG << retVal;
        return false;
    }else{
#ifdef APIS_DEBUG_MODE
        LOG_DEBUG << retVal;
#endif
        return true;
    }
}

QString AutoFarmerAPIsWraper::w_screenCapture(const QString fullPath)
{
    QJsonObject retVal = this->screenCapture(fullPath);
    return retVal["FilePath"].toString();
}

bool AutoFarmerAPIsWraper::w_installPackage(QString apkPath)
{
    QJsonObject retVal = this->installPackage(apkPath);
    return retVal["Status"].toBool();
}

bool AutoFarmerAPIsWraper::w_isExistPackage(QString packageName)
{
    QJsonObject retVal = this->isExistPackage(packageName);
    if(retVal["Status"].toBool() == false){
        LOG_DEBUG << retVal;
        return false;
    }else{
#ifdef APIS_DEBUG_MODE
        LOG_DEBUG << retVal;
#endif
        return true;
    }
}

bool AutoFarmerAPIsWraper::w_uninstallPackage(QString packageName)
{
    QJsonObject retVal = this->uninstallPackage(packageName);
    if(retVal["Status"].toBool() == false){
        LOG_DEBUG << retVal;
        return false;
    }else{
#ifdef APIS_DEBUG_MODE
        LOG_DEBUG << retVal;
#endif
        return true;
    }
}

bool AutoFarmerAPIsWraper::w_openPackage(QString packageName, QString activity)
{
    QJsonObject retVal = this->openPackage(packageName,activity);
    if(retVal["Status"].toBool() == false){
        LOG_DEBUG << retVal;
        return false;
    }else{
#ifdef APIS_DEBUG_MODE
        LOG_DEBUG << retVal;
#endif
        return true;
    }
}

bool AutoFarmerAPIsWraper::w_closePackage(QString packageName)
{
    QJsonObject retVal = this->closePackage(packageName);
    if(retVal["Status"].toBool() == false){
        LOG_DEBUG << retVal;
        return false;
    }else{
#ifdef APIS_DEBUG_MODE
        LOG_DEBUG << retVal;
#endif
        return true;
    }
}

bool AutoFarmerAPIsWraper::w_wipePackage(QStringList packageNames)
{
    QJsonObject retVal = this->wipePackage(packageNames);
    if(retVal["Status"].toBool() == false){
        LOG_DEBUG << retVal;
        return false;
    }else{
#ifdef APIS_DEBUG_MODE
        LOG_DEBUG << retVal;
#endif
        return true;
    }
}

/* This method has not wrapped yet */
QList<TEXT_COMPOENT> AutoFarmerAPIsWraper::w_getTextFromImage(QString imagePath)
{
    QList<TEXT_COMPOENT> textCmpList;
    textCmpList.clear();
    QJsonObject retVal = this->getTextFromImage(imagePath);
    if(retVal["Status"].toBool() == false){
        LOG_DEBUG << retVal;
        return textCmpList;
    }else{
#ifdef APIS_DEBUG_MODE
        LOG_DEBUG << retVal;
#endif
        foreach (QJsonValue jsonValue, retVal["ResponseData"].toArray()) {
            TEXT_COMPOENT component;
            QJsonObject obj = jsonValue.toObject();
            component.x = obj["x"].toInt();
            component.y = obj["y"].toInt();
            component.width = obj["width"].toInt();
            component.height = obj["height"].toInt();
            component.confidence = obj["confidence"].toInt();
            component.text = obj["text"].toString().toLower();
            textCmpList.append(component);
        };
        return textCmpList;
    }
}

QPoint AutoFarmerAPIsWraper::w_findImageFromImage(QString templatePath, QString imagePath)
{
    QJsonObject retVal = this->findImageFromImage(templatePath,imagePath);
    return QPoint(retVal["X"].toInt(),retVal["Y"].toInt());
}

bool AutoFarmerAPIsWraper::findAndClick(QString iconPath)
{
    bool retVal = false;
    QString screenImgPath = this->w_screenCapture(SCREEN_IMG);
    if(iconPath == "") {
        LOG_DEBUG << "iconpath is Empty";
        retVal = false;
    }
    else if(screenImgPath == "") {
        LOG_DEBUG << "screenImgPath is Empty";
        retVal = false;
    }
    else {
        QPoint point = this->w_findImageFromImage(iconPath,screenImgPath);
        if(point.isNull()){
            retVal = false;
        }else{
            retVal = this->w_doClick(point.x(), point.y());
        }
    }
    LOG_DEBUG << iconPath.split("/").last() << " : " << retVal;
    return retVal;
}

bool AutoFarmerAPIsWraper::enterKeyBoard()
{
    return this->w_inputKeyEvent("KEYCODE_ENTER");
}
