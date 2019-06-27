#include "Model.h"
#include <QWidgetList>
#include <QtWidgets/qwidget.h>
#include <QWindow>

Model* Model::m_instance = nullptr;

Model::Model(QObject *parent) : QObject(parent)
{
    m_token = "c6a52e63a8d52869b80af5af1a22c2b5";
    m_currentPkgIndex = -1;
    this->loadUserDataList();
}


Model *Model::instance()
{
    if(m_instance == nullptr){
        m_instance = new Model();
    }
    return m_instance;
}

QJsonDocument Model::loadJson(QString fileName)
{
    LOG << "[Model]";
    QFile jsonFile(fileName);
    jsonFile.open(QFile::ReadOnly);
    return QJsonDocument().fromJson(jsonFile.readAll());
}

void Model::saveJson(QJsonDocument document, QString fileName)
{
    LOG <<"document: " << document;
    LOG <<"fileName: " << fileName;
    QFile jsonFile(fileName);
    jsonFile.open(QFile::WriteOnly);
    jsonFile.write(document.toJson());
}

void Model::start()
{
    emit sigStartProgram();
}

QString Model::token() const
{
    return m_token;
}

void Model::setToken(QString data)
{
    if(m_token != data){
        m_token = data;
        emit tokenChanged();
    }
}

DEVICE_INFO Model::deviceInfo() const
{
    return m_deviceInfo;
}

void Model::setDeviceInfo(DEVICE_INFO data)
{
    m_deviceInfo = data;
}

APP_CONFIG Model::appConfig() const
{
    return m_appConfig;
}

void Model::setAppConfig(APP_CONFIG data)
{
    m_appConfig = data;
}

USER_DATA Model::userData() const
{
    return m_userData;
}

void Model::setUserData(USER_DATA data)
{
    m_userData = data;
}

void Model::updateUserData(QString packageName, USER_DATA data)
{
    if(m_userDataList.contains(packageName)){
        m_userDataList[packageName] = data;
    }else{
        LOG << "Add new pacakge: " << packageName;
        m_userDataList.insert(packageName,data);
    }
}

QMap<QString, USER_DATA>& Model::getUserDataList()
{
    return m_userDataList;
}

QString Model::currentControlledPkg()
{
    if(m_currentPkgIndex < m_userDataList.count()){
        return m_userDataList.keys().at(m_currentPkgIndex);
    }else{
        LOG << "m_currentPkgIndex is invalid";
        return "";
    }
}

USER_DATA &Model::currentControlledUser()
{
    return m_userDataList[currentControlledPkg()];
}

void Model::nextCurrentControlledObj()
{
    if(m_currentPkgIndex < 0 || m_currentPkgIndex >= m_userDataList.count() - 1){
        m_currentPkgIndex = 0;
    }else{
        m_currentPkgIndex ++;
    }
    emit nextCurrentControlledObjChanged();
}

QList<QJsonObject> Model::actionList()
{
    return m_actionList;
}

void Model::setActionList(QList<QJsonObject> data)
{
    if(m_actionList!= data){
        m_actionList = data;
        m_changedActionList = m_actionList;
        LOG << "----------------------- New circle ------------------------";
        emit currentActionChanged();
    }
}

void Model::clearActionList()
{
    m_actionList.clear();
}

QJsonObject Model::currentAction()
{
    if(m_changedActionList.isEmpty()){
        return QJsonObject();
    }else{
        return m_changedActionList.first();
    }
}

void Model::nextCurrentAction()
{
    LOG;
    if(m_changedActionList.isEmpty()){
        nextCurrentControlledObj();
    }else if(m_changedActionList.length() == 1){
        m_changedActionList.removeFirst();
        nextCurrentControlledObj();
    }else{
        m_changedActionList.removeFirst();
    }
    emit currentActionChanged();
}

void Model::loadUserDataList()
{
    LOG;
    m_userDataList.clear();

    QFile file(USER_DATA_LIST_PATH);
    if(file.exists()){

        QJsonDocument userDataListJson = this->loadJson(USER_DATA_LIST_PATH);
        if(!userDataListJson.isNull()){
            foreach (QJsonValue data, userDataListJson.array()) {
                if(data.isObject()){
                    QJsonObject obj = data.toObject();
                    QString packageName = obj["package_name"].toString();
                    QJsonObject userDataJson = obj["user_data"].toObject();
                    USER_DATA user_data;
                    user_data._id              = userDataJson["_id"].toString();
                    user_data.uid              = userDataJson["uid"].toString();
                    user_data.password         = userDataJson["password"].toString();
                    user_data.cookie           = userDataJson["cookie"].toString();
                    user_data.token            = userDataJson["token"].toString();
                    user_data.birthday         = userDataJson["birthday"].toString();
                    user_data.name             = userDataJson["name"].toString();
                    user_data.sex              = userDataJson["sex"].toString();
                    user_data.country          = userDataJson["country"].toString();
                    user_data.email            = userDataJson["email"].toString();
                    user_data.avartar          = userDataJson["avartar"].toString();
                    user_data.created_date     = userDataJson["created_date"].toString();
                    user_data.farming_status   = userDataJson["farming_status"].toString();
                    user_data.alive_status     = userDataJson["alive_status"].toString();
                    user_data.created_at       = userDataJson["created_at"].toString();
                    user_data.updated_at       = userDataJson["updated_at"].toString();
                    user_data.user_id          = userDataJson["user_id"].toString();
                    m_userDataList.insert(packageName,user_data);
                }
            }
        }
    }else{
        LOG << CURRENT_DIR + "userDataList.json" << " not exist";
    }
}

void Model::saveUserDataList()
{
    LOG;
    QMap<QString, USER_DATA>::const_iterator i = m_userDataList.constBegin();
    QJsonArray objectArray;
    while (i != m_userDataList.constEnd()) {
        LOG << i.key() << ": " << static_cast<USER_DATA>(i.value())._id;
        QJsonObject obj;
        USER_DATA user_data = static_cast<USER_DATA>(i.value());
        QJsonObject userDataJson;
        userDataJson["_id"]             = user_data._id           ;
        userDataJson["uid"]             = user_data.uid           ;
        userDataJson["password"]        = user_data.password      ;
        userDataJson["cookie"]          = user_data.cookie        ;
        userDataJson["token"]           = user_data.token         ;
        userDataJson["birthday"]        = user_data.birthday      ;
        userDataJson["name"]            = user_data.name          ;
        userDataJson["sex"]             = user_data.sex           ;
        userDataJson["country"]         = user_data.country       ;
        userDataJson["email"]           = user_data.email         ;
        userDataJson["avartar"]         = user_data.avartar       ;
        userDataJson["created_date"]    = user_data.created_date  ;
        userDataJson["farming_status"]  = user_data.farming_status;
        userDataJson["alive_status"]    = user_data.alive_status  ;
        userDataJson["created_at"]      = user_data.created_at    ;
        userDataJson["updated_at"]      = user_data.updated_at    ;
        userDataJson["user_id"]         = user_data.user_id       ;

        obj["package_name"] = i.key();
        obj["user_data"] = userDataJson;
        objectArray.append(QJsonValue(obj));
        ++i;
    }
    this->saveJson(QJsonDocument(objectArray),USER_DATA_LIST_PATH);
}



