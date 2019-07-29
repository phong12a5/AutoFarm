#include "Model.hpp"
#include <QWidgetList>
#include "Processing/ImageProcessing.hpp"
#include "Controller/ShellOperation.hpp"

Model* Model::m_instance = nullptr;

Model::Model(QObject *parent) : QObject(parent)
{
    m_token = "d35c093ccdb9926676e5bf4e3b78e66c";
    m_currentPkgIndex = -1;
    this->loadUserDataList();
    foreach (QString key, m_userDataList.keys()) {
        LOG << "Key: " << key << " -- " << m_userDataList.value(key).uid;
    }
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
    if(testImageMode()){
        delay(5000);
        QDir screenshotDir =  QDir(QDir::currentPath() + "/images/Screenshots");
        screenshotDir.setNameFilters(QStringList() << "*.png");

        QDir iconDir =  QDir(QDir::currentPath() + "/images/Icons");
        iconDir.setNameFilters(QStringList() << "*.png");

        foreach (QFileInfo file, screenshotDir.entryInfoList()) {
            Model::instance()->setTestingImageSource(file.absoluteFilePath());
            Model::instance()->setScreenName("");

            LOG << Model::instance()->testingImageSource();

            QTimer timer;
            timer.setInterval(2000);
            timer.setSingleShot(true);

            QEventLoop event;
            connect(&timer, SIGNAL(timeout()), &event, SLOT(quit()));
            timer.start();
            event.exec();

            foreach (QFileInfo icon, iconDir.entryInfoList()) {
                QString iconName = icon.absoluteFilePath();
                LOG << iconName;
                QPoint result = ImageProcessing::findImageOnImage(iconName, Model::instance()->testingImageSource());
                if(!result.isNull()){
                    Model::instance()->setScreenName(Model::instance()->screenName() + "\n" + iconName.split("/").last());
                }

                QTimer timer2;
                timer2.setInterval(100);
                timer2.setSingleShot(true);

                QEventLoop event2;
                connect(&timer2, SIGNAL(timeout()), &event2, SLOT(quit()));

                if(icon == iconDir.entryInfoList().last()){
                    Model::instance()->setScreenName(Model::instance()->screenName() + "\n Done");
                    timer2.setInterval(10000);
                }

                timer2.start();
                event2.exec();
            }
        }
        delay(2000);
    }else{
        emit sigStartProgram();
    }
}

QString Model::token() const
{
    return m_token;
}

void Model::setToken(QString data)
{
    LOG << data;
    if(m_token != data){
        m_token = data;
        this->saveAppConfig();
        emit tokenChanged();
    }
}

QString Model::deviceType() const
{
    if(m_deviceInfo.isNox == "true")
        return "Nox Device";
    else
        return "Real Device";
}

DEVICE_INFO Model::deviceInfo() const
{
    return m_deviceInfo;
}

void Model::setDeviceInfo(DEVICE_INFO data)
{
    m_deviceInfo = data;
}

QString Model::logContent()
{
    return m_logContent;
}

void Model::setLogContent(QString data)
{
    m_logContent += (data + "\n") ;
    emit logContentChanged();
}

APP_CONFIG Model::appConfig() const
{
    return m_appConfig;
}

void Model::setAppConfig(APP_CONFIG data)
{
    m_appConfig = data;
}

QMap<QString, USER_DATA>* Model::getUserDataList()
{
    return &m_userDataList;
}

QString Model::currentControlledPkg()
{
    LOG << m_currentPkgIndex;
    if(m_currentPkgIndex < m_userDataList.count()){
        return m_userDataList.keys().at(m_currentPkgIndex);
    }else{
        LOG << "m_currentPkgIndex is invalid";
        return "";
    }
}

USER_DATA Model::currentControlledUser()
{
    LOG << m_userDataList.value(currentControlledPkg()).uid;
    return m_userDataList.value(currentControlledPkg());
}

void Model::updateCurrentControlleredUser(USER_DATA data)
{
    if(m_userDataList[currentControlledPkg()] != data){
        m_userDataList[currentControlledPkg()] = data;
        saveUserDataList();
    }
}

void Model::nextCurrentControlledObj()
{

    if(m_currentPkgIndex >= 0 && m_currentPkgIndex < m_userDataList.count())
        ShellOperation::stopApp(this->currentControlledPkg());

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
    LOG << "m_currentPkgIndex: " << m_currentPkgIndex;
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
    if(m_changedActionList.length() <= 1){
        if(m_changedActionList.length() == 1){
            m_changedActionList.removeFirst();
        }
        emit currentActionListDone();
        nextCurrentControlledObj();
    }else{
        m_changedActionList.removeFirst();
    }
    emit currentActionChanged();
}

QStringList Model::neededInstallpackageList()
{
    return m_neededInstallpackageList;
}

void Model::setNeededInstallpackageList(QStringList data)
{
    if(m_neededInstallpackageList != data){
        m_neededInstallpackageList = data;
    }
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

//                    LOG << "user_data._id           :" << user_data._id           ;
                    LOG << "user_data.uid           :" << user_data.uid           ;
//                    LOG << "user_data.password      :" << user_data.password      ;
//                    LOG << "user_data.cookie        :" << user_data.cookie        ;
//                    LOG << "user_data.token         :" << user_data.token         ;
//                    LOG << "user_data.birthday      :" << user_data.birthday      ;
//                    LOG << "user_data.name          :" << user_data.name          ;
//                    LOG << "user_data.sex           :" << user_data.sex           ;
//                    LOG << "user_data.country       :" << user_data.country       ;
//                    LOG << "user_data.email         :" << user_data.email         ;
//                    LOG << "user_data.avartar       :" << user_data.avartar       ;
//                    LOG << "user_data.created_date  :" << user_data.created_date  ;
//                    LOG << "user_data.farming_status:" << user_data.farming_status;
//                    LOG << "user_data.alive_status  :" << user_data.alive_status  ;
//                    LOG << "user_data.created_at    :" << user_data.created_at    ;
//                    LOG << "user_data.updated_at    :" << user_data.updated_at    ;
//                    LOG << "user_data.user_id       :" << user_data.user_id       ;

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

//        LOG << "user_data._id           :" << user_data._id           ;
//        LOG << "user_data.uid           :" << user_data.uid           ;
//        LOG << "user_data.password      :" << user_data.password      ;
//        LOG << "user_data.cookie        :" << user_data.cookie        ;
//        LOG << "user_data.token         :" << user_data.token         ;
//        LOG << "user_data.birthday      :" << user_data.birthday      ;
//        LOG << "user_data.name          :" << user_data.name          ;
//        LOG << "user_data.sex           :" << user_data.sex           ;
//        LOG << "user_data.country       :" << user_data.country       ;
//        LOG << "user_data.email         :" << user_data.email         ;
//        LOG << "user_data.avartar       :" << user_data.avartar       ;
//        LOG << "user_data.created_date  :" << user_data.created_date  ;
//        LOG << "user_data.farming_status:" << user_data.farming_status;
//        LOG << "user_data.alive_status  :" << user_data.alive_status  ;
//        LOG << "user_data.created_at    :" << user_data.created_at    ;
//        LOG << "user_data.updated_at    :" << user_data.updated_at    ;
//        LOG << "user_data.user_id       :" << user_data.user_id       ;

        obj["package_name"] = i.key();
        obj["user_data"] = userDataJson;
        objectArray.append(QJsonValue(obj));
        ++i;
    }
    this->saveJson(QJsonDocument(objectArray),USER_DATA_LIST_PATH);
}

void Model::loadAppConfig()
{
    QFile file(CONFIG_FILE_PATH);
    if(file.exists()){

        QJsonDocument appConfig = this->loadJson(CONFIG_FILE_PATH);
        if(!appConfig.isNull()){
            QJsonObject obj = appConfig.object();
            LOG << "AppConfig: " << obj;
            QString _token = obj[TOKEN_PROP_KEY].toString();
            if(!_token.isEmpty()){
                this->setToken(_token);
            }
        }
    }else{
        LOG << CONFIG_FILE_PATH << " not exist";
    }
}

void Model::saveAppConfig()
{
    LOG;
    QJsonObject obj;
    obj[TOKEN_PROP_KEY] = this->token();
    this->saveJson(QJsonDocument(obj),CONFIG_FILE_PATH);
}

QString Model::testingImageSource() const
{
    return m_testingImageSource;
}

void Model::setTestingImageSource(QString data)
{
    if(m_testingImageSource != data){
        m_testingImageSource = data;
        emit testingImageSourceChanged();
    }
}

QString Model::screenName() const
{
    return m_screenName;
}

void Model::setScreenName(QString data)
{
    if(m_screenName != data){
        m_screenName = data;
        emit screenNameChanged();
    }
}

bool Model::testImageMode()
{
    return false;
}



