#include "Model.hpp"
#include <QWidgetList>

Model* Model::m_instance = nullptr;

Model::Model(QObject *parent) : QObject(parent)
{
    m_token = "00261f5687fee223f35e3c2080e167a8";
    m_currentPkgIndex = -1;
    m_autoStart = true;
    m_currentScreen = -1;

//    QString _token;
//    ShellOperation::shellCommand(QString("getprop %1").arg(TOKEN_PROP_KEY),_token);
//    _token.simplified();

//    LOG_DEBUG << "_token: " << _token;

//    if(!_token.isEmpty() && _token.contains("@")){
//        setToken(_token.section("@",1,1));
//    }

}


Model *Model::instance()
{
    if(m_instance == nullptr){
        m_instance = new Model();
    }
    return m_instance;
}

void Model::initModel()
{
    this->loadAppConfig();
}

QJsonDocument Model::loadJson(QString fileName)
{
    LOG_DEBUG << "[Model]";
    QFile jsonFile(fileName);
    jsonFile.open(QFile::ReadOnly);
    return QJsonDocument().fromJson(jsonFile.readAll());
}

void Model::saveJson(QJsonDocument document, QString fileName)
{
    LOG_DEBUG <<"document: " << document;
    LOG_DEBUG <<"fileName: " << fileName;
    QFile jsonFile(fileName);
    jsonFile.open(QFile::WriteOnly);
    jsonFile.write(document.toJson());
}

void Model::start()
{
    this->saveAppConfig();
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
        this->saveAppConfig();
        emit tokenChanged();
    }
}

bool Model::autoStart() const
{
    return m_autoStart;
}

void Model::setAutoStart(bool data)
{
    if(m_autoStart != data){
        m_autoStart = data;
        this->saveAppConfig();
        emit autoStartChanged();
    }
}

QString Model::deviceType() const
{
    if(m_deviceInfo.isNox == "true")
        return "Nox Device";
    else
        return "Real Device";
}

bool Model::isNoxDevice() const
{
    if(m_deviceInfo.isNox == "true")
        return true;
    else
        return false;
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

QMap<QString, USER_DATA> Model::userDataList()
{
    return m_userDataList;
}

void Model::setUserDataList(QMap<QString, USER_DATA> data)
{
    m_userDataList = data;
}


QString Model::currentControlledPkg()
{
    LOG_DEBUG << m_currentPkgIndex;
    if(m_currentPkgIndex < m_userDataList.count() && m_currentPkgIndex >= 0){
        return m_userDataList.keys().at(m_currentPkgIndex);
    }else{
        LOG_DEBUG << "m_currentPkgIndex is invalid";
        return "";
    }
}

USER_DATA Model::currentControlledUser()
{
    if(currentControlledPkg() != ""){
        LOG_DEBUG << m_userDataList.value(currentControlledPkg()).uid;
        return m_userDataList.value(currentControlledPkg());
    }else{
        USER_DATA data;
        LOG_DEBUG << "currentControlledPkg is NULL";
        return data;
    }
}

void Model::updateCurrentControlleredUser(USER_DATA data)
{
    if(m_userDataList[currentControlledPkg()] != data){
        m_userDataList[currentControlledPkg()] = data;
    }
}

void Model::nextCurrentControlledObj()
{
    if(m_currentPkgIndex < 0 || m_currentPkgIndex >= m_userDataList.count() - 1){
        if(m_currentPkgIndex >= m_userDataList.count() - 1){
            emit finishedListObject();
            m_currentPkgIndex = 0;
            emit nextCurrentControlledObjChanged();
            return;
        }
        m_currentPkgIndex = 0;
    }else{
        m_currentPkgIndex ++;
    }
    LOG_DEBUG << "m_currentPkgIndex: " << m_currentPkgIndex;
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
    }
}

void Model::clearActionList()
{
    m_actionList.clear();
}

void Model::loadAppConfig()
{
    QFile file(CONFIG_FILE_PATH);
    if(file.exists()){

        QJsonDocument appConfig = this->loadJson(CONFIG_FILE_PATH);
        if(!appConfig.isNull()){
            QJsonObject obj = appConfig.object();
            LOG_DEBUG << "AppConfig: " << obj;
            QString _token = obj[TOKEN_PROP_KEY].toString();
            if(!_token.isEmpty()){
                this->setToken(_token);
            }
            this->setAutoStart(obj[AUTO_START_KEY].toBool());
        }
    }else{
        LOG_DEBUG << CONFIG_FILE_PATH << " not exist";
    }
}

void Model::saveAppConfig()
{
    LOG_DEBUG;
    QJsonObject obj;
    obj[TOKEN_PROP_KEY] = this->token();
    obj[AUTO_START_KEY] = this->autoStart();
    this->saveJson(QJsonDocument(obj),CONFIG_FILE_PATH);
}

int Model::currentScreen() const
{
    return m_currentScreen;
}

void Model::setCurrentScreen(const int data)
{
    if(m_currentScreen != data){
        m_currentScreen = data;
        emit currentScreenChanged();
    }
}

QString Model::screenStr(int screenID) const
{
    QString retVal = "";
    switch (screenID) {
    case AppEnums::HMI_UNKNOW_SCREEN:
        retVal = "HMI_UNKNOW_SCREEN";
        break;
    case AppEnums::HMI_SELECT_LANGUAGE_SCREEN:
        retVal = "HMI_SELECT_LANGUAGE_SCREEN";
        break;
    case AppEnums::HMI_LOGIN_SCREEN:
        retVal = "HMI_LOGIN_SCREEN";
        break;
    case AppEnums::HMI_INCORRECT_PASSWORD_SCREEN:
        retVal = "HMI_INCORRECT_PASSWORD_SCREEN";
        break;
    case AppEnums::HMI_MISSING_PASSWORD_SCREEN:
        retVal = "HMI_MISSING_PASSWORD_SCREEN";
        break;
    case AppEnums::HMI_CONFIRM_INDENTIFY_SCREEN:
        retVal = "HMI_CONFIRM_INDENTIFY_SCREEN";
        break;
    case AppEnums::HMI_DEACTIVE_ACCOUNT_SCREEN:
        retVal = "HMI_DEACTIVE_ACCOUNT_SCREEN";
        break;
    case AppEnums::HMI_TURNON_FIND_FRIEND_SCREEN:
        retVal = "HMI_TURNON_FIND_FRIEND_SCREEN";
        break;
    case AppEnums::HMI_SAVE_LOGIN_INFO_SCREEN:
        retVal = "HMI_SAVE_LOGIN_INFO_SCREEN";
        break;
    case AppEnums::HMI_CHOOSE_AVATAR_SCREEN:
        retVal = "HMI_CHOOSE_AVATAR_SCREEN";
        break;
    case AppEnums::HMI_ADDFRIEND_SUGGESTION_SCREEN:
        retVal = "HMI_ADDFRIEND_SUGGESTION_SCREEN";
        break;
    case AppEnums::HMI_NEW_FEED_SCREEN:
        retVal = "HMI_NEW_FEED_SCREEN";
        break;
    case AppEnums::HMI_LOGIN_AGAIN_SCREEN:
        retVal = "HMI_LOGIN_AGAIN_SCREEN";
        break;
    case AppEnums::HMI_ACCOUNT_NOT_FOUND_SCREEN:
        retVal = "HMI_ACCOUNT_NOT_FOUND_SCREEN";
        break;
    default:
        break;
    }
    return retVal;
}
