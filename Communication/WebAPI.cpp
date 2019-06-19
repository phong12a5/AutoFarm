#include "WebAPI.h"
#include "Model.h"

#define MODEL Model::instance()

WebAPI::WebAPI(QObject *parent) : QObject(parent)
{

}

void WebAPI::saveFile(QString fileName, QByteArray content)
{
    if(content.isEmpty()){
        LOG << "Content is empty! >> EXIT!";
        return;
    }else{
        QString pathName = QDir::currentPath() + "/" + fileName;
        LOG << "pathName: " << pathName;
        QFile *file = new QFile(pathName);
        if(file->open(QIODevice::WriteOnly | QIODevice::Text))
        {
            LOG << "Open file ok >> " << file->symLinkTarget();
            file->write(content);
            file->flush();
            file->close();
        }else{
            LOG << "Failed to open file";
        }
        delete file;
    }
}

void WebAPI::getApk()
{
    QString url = API_SERVER + QString("config?token=%1").arg(MODEL->token());
    QUrl serviceUrl = QUrl(url);
    QNetworkRequest request(serviceUrl);
    QJsonObject json;

    json.insert("action", QTextCodec::codecForMib(106)->toUnicode(getEncodedString("getapk")));
    json.insert("device", QTextCodec::codecForMib(106)->toUnicode(getEncodedDeviceInfo()));

    LOG << "json: " << json;

    QByteArray jsonData = QJsonDocument(json).toJson();
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");
    request.setHeader(QNetworkRequest::ContentLengthHeader,QByteArray::number(jsonData.size()));
    QNetworkAccessManager* networkManager = new QNetworkAccessManager(this);
    QObject::connect(networkManager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(slotReponseGettingApk(QNetworkReply*)));
    networkManager->post(request, jsonData);
}

void WebAPI::getConfig()
{
    QString url = API_SERVER + QString("config?token=%1").arg(MODEL->token());
    QUrl serviceUrl = QUrl(url);
    QNetworkRequest request(serviceUrl);
    QJsonObject json;

    json.insert("action", QTextCodec::codecForMib(106)->toUnicode(getEncodedString("config")));
    json.insert("device", QTextCodec::codecForMib(106)->toUnicode(getEncodedDeviceInfo()));

    LOG << "json: " << json;

    QByteArray jsonData = QJsonDocument(json).toJson();
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");
    request.setHeader(QNetworkRequest::ContentLengthHeader,QByteArray::number(jsonData.size()));
    QNetworkAccessManager* networkManager = new QNetworkAccessManager(this);
    QObject::connect(networkManager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(slotReponseGettingConfig(QNetworkReply*)));
    networkManager->post(request, jsonData);
}

void WebAPI::cloneUser()
{
    QString url = API_SERVER + QString("clone?token=%1").arg(MODEL->token());
    QUrl serviceUrl = QUrl(url);
    QNetworkRequest request(serviceUrl);

    QNetworkAccessManager* networkManager = new QNetworkAccessManager(this);
    QObject::connect(networkManager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(slotReponseCloningConfig(QNetworkReply*)));
    networkManager->get(request);
}

void WebAPI::updateCheckPoint()
{
    QString url = API_SERVER + QString("clone?token=%1").arg(MODEL->token());
    QUrl serviceUrl = QUrl(url);
    QNetworkRequest request(serviceUrl);
    QJsonObject json;

    json.insert("action", QTextCodec::codecForMib(106)->toUnicode(getEncodedString("checkpoint")));
    json.insert("device", QTextCodec::codecForMib(106)->toUnicode(getEncodedDeviceInfo()));
    json.insert("data", QTextCodec::codecForMib(106)->toUnicode(getEncodedStringByImei(MODEL->userData().uid)));

    LOG << "json: " << json;

    QByteArray jsonData = QJsonDocument(json).toJson();
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");
    request.setHeader(QNetworkRequest::ContentLengthHeader,QByteArray::number(jsonData.size()));
    QNetworkAccessManager* networkManager = new QNetworkAccessManager(this);
    QObject::connect(networkManager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(slotReponseUpdatingCheckpoint(QNetworkReply*)));
    networkManager->post(request, jsonData);
}

void WebAPI::downloadApk(QUrl url)
{
    QNetworkAccessManager* networkManager = new QNetworkAccessManager(this);
    QObject::connect(networkManager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(slotReponseDownloadingApk(QNetworkReply*)));
    networkManager->get(QNetworkRequest(QUrl(url)));
}

QString WebAPI::getKeyByToken() const
{
    return MODEL->token() + "congaubeo@123";
}

QString WebAPI::getKeyByIMEI() const
{
    return MODEL->deviceInfo().iMEI + MODEL->deviceInfo().iMEI + MODEL->deviceInfo().iMEI + MODEL->deviceInfo().iMEI;
}

QString WebAPI::getIV() const
{
    return "0123456789012345";
}

QByteArray WebAPI::getEncodedDeviceInfo() const
{
    QJsonObject deviceInfo;

    deviceInfo["GoogleSF"] =        MODEL->deviceInfo().googleSF;
    deviceInfo["AndroidID"] =       MODEL->deviceInfo().androidID;
    deviceInfo["IMEI"] =            MODEL->deviceInfo().iMEI;
    deviceInfo["IMSI"] =            MODEL->deviceInfo().iMSI;
    deviceInfo["SIMCardSerial"] =   MODEL->deviceInfo().sIMCardSerial;
    deviceInfo["WifiMacAddress"] =  MODEL->deviceInfo().wifiMacAddress;
    deviceInfo["android_verion"] =  MODEL->deviceInfo().android_verion;
    deviceInfo["model"] =           MODEL->deviceInfo().model;

    LOG << "deviceInfo: " << deviceInfo;

    QByteArray deviceInfoData = QJsonDocument(deviceInfo).toJson();

    QAESEncryption encryption(QAESEncryption::AES_256, QAESEncryption::CBC);

    return encryption.encode(deviceInfoData, getKeyByToken().toLocal8Bit(), getIV().toLocal8Bit()).toBase64();
}

QByteArray WebAPI::getEncodedString(QString action) const
{
    QAESEncryption encryption(QAESEncryption::AES_256, QAESEncryption::CBC);
    return encryption.encode(action.toLocal8Bit(), getKeyByToken().toLocal8Bit(), getIV().toLocal8Bit()).toBase64();
}

QByteArray WebAPI::getEncodedStringByImei(QString action) const
{
    QAESEncryption encryption(QAESEncryption::AES_256, QAESEncryption::CBC);
    return encryption.encode(action.toLocal8Bit(), getKeyByIMEI().toLocal8Bit(), getIV().toLocal8Bit()).toBase64();
}

void WebAPI::slotReponseGettingApk(QNetworkReply* reply)
{
    QByteArray responseData = reply->readAll();
    LOG << "QJsonDocument::fromJson(responseData): " << QJsonDocument::fromJson(responseData);
    QJsonObject jsonObj = QJsonDocument::fromJson(responseData).object();

    if(jsonObj.isEmpty()){
        LOG << "jsonObj is empty!";
        return;
    }else{
        // Continue
    }

    if(jsonObj["action"].toString() == QTextCodec::codecForMib(106)->toUnicode(getEncodedString("getapk"))){
        QString data =  jsonObj["data"].toString();

        QAESEncryption encryption(QAESEncryption::AES_256, QAESEncryption::CBC);
        QByteArray decodeText = encryption.decode(QByteArray::fromBase64(data.toUtf8()), getKeyByIMEI().toLocal8Bit(), getIV().toLocal8Bit());
        QJsonDocument jdoc = QJsonDocument::fromJson(encryption.removePadding(decodeText));

        foreach (QJsonValue data, jdoc.array()) {
            if(data.isObject()){
                QJsonObject obj = data.toObject();
                this->downloadApk(QUrl(obj["apk"].toString()));
            }
        }
    }else{
        LOG << "Another action!";
    }
}

void WebAPI::slotReponseGettingConfig(QNetworkReply * reply)
{
    QByteArray responseData = reply->readAll();
    LOG << "QJsonDocument::fromJson(responseData): " << QJsonDocument::fromJson(responseData);
    QJsonObject jsonObj = QJsonDocument::fromJson(responseData).object();

    if(jsonObj.isEmpty()){
        LOG << "jsonObj is empty!";
        return;
    }else{
        // Continue
    }

    if(jsonObj["action"].toString() == QTextCodec::codecForMib(106)->toUnicode(getEncodedString("config"))){
        QString data =  jsonObj["data"].toString();

        QAESEncryption encryption(QAESEncryption::AES_256, QAESEncryption::CBC);
        QByteArray decodeText = encryption.decode(QByteArray::fromBase64(data.toUtf8()), getKeyByIMEI().toLocal8Bit(), getIV().toLocal8Bit());
        QJsonDocument jdoc = QJsonDocument::fromJson(encryption.removePadding(decodeText));
        QJsonObject jsonObj = jdoc.object();
        if(!jsonObj.isEmpty()){
            APP_CONFIG config;
            config.timeout =    jsonObj["Timeout"].toInt();
            config.reset_3g =   jsonObj["Reset3g"].toInt();
            config.debug_mode = jsonObj["Debug_mode"].toString();
            config.user_type = jsonObj["User_type"].toInt();
            config.encryption = jsonObj["Encryption"].toString();
            LOG << "config.timeout: " << config.timeout;
            LOG << "config.reset_3g: " << config.reset_3g;
            LOG << "config.debug_mode: " << config.debug_mode;
            LOG << "config.user_type: " << config.user_type;
            LOG << "config.encryption: " << config.encryption;
            MODEL->setAppConfig(config);
        }

    }else{
        LOG << "Another action!";
    }
}

void WebAPI::slotReponseCloningConfig(QNetworkReply * reply)
{
    QByteArray responseData = reply->readAll();

    QAESEncryption encryption(QAESEncryption::AES_256, QAESEncryption::CBC);
    QByteArray decodeText = encryption.decode(QByteArray::fromBase64(responseData), getKeyByIMEI().toLocal8Bit(), getIV().toLocal8Bit());
    QJsonDocument jdoc = QJsonDocument::fromJson(encryption.removePadding(decodeText));
    QJsonObject jsonObj = jdoc.object();
    if(!jsonObj.isEmpty()){
        USER_DATA user_data;
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

        LOG << "user_data._id           :" << user_data._id           ;
        LOG << "user_data.uid           :" << user_data.uid           ;
        LOG << "user_data.password      :" << user_data.password      ;
        LOG << "user_data.cookie        :" << user_data.cookie        ;
        LOG << "user_data.token         :" << user_data.token         ;
        LOG << "user_data.birthday      :" << user_data.birthday      ;
        LOG << "user_data.name          :" << user_data.name          ;
        LOG << "user_data.sex           :" << user_data.sex           ;
        LOG << "user_data.country       :" << user_data.country       ;
        LOG << "user_data.email         :" << user_data.email         ;
        LOG << "user_data.avartar       :" << user_data.avartar       ;
        LOG << "user_data.created_date  :" << user_data.created_date  ;
        LOG << "user_data.farming_status:" << user_data.farming_status;
        LOG << "user_data.alive_status  :" << user_data.alive_status  ;
        LOG << "user_data.created_at    :" << user_data.created_at    ;
        LOG << "user_data.updated_at    :" << user_data.updated_at    ;
        LOG << "user_data.user_id       :" << user_data.user_id       ;

        MODEL->setUserData(user_data);
    }else{
        LOG << "jsonObj is NULL";
    }
}

void WebAPI::slotReponseUpdatingCheckpoint(QNetworkReply * reply)
{
    QByteArray responseData = reply->readAll();

    QAESEncryption encryption(QAESEncryption::AES_256, QAESEncryption::CBC);
    QByteArray decodeText = encryption.decode(QByteArray::fromBase64(responseData), getKeyByIMEI().toLocal8Bit(), getIV().toLocal8Bit());
    QJsonDocument jdoc = QJsonDocument::fromJson(encryption.removePadding(decodeText));
    QJsonObject jsonObj = jdoc.object();
    if(!jsonObj.isEmpty()){
        LOG << "Updated: " << jsonObj["updated"].toBool();
    }else{
        LOG << "jsonObj is NULL";
    }
}

void WebAPI::slotReponseDownloadingApk(QNetworkReply * reply)
{
    if(reply->error()){
        LOG << "ERROR: " << reply->errorString();
    }else{
        QString url =  reply->url().toString();
        LOG << "Url: " << url;
        if(url.split("/").last().contains("com.facebook")){
            QString fileName = url.split("/").last();
            this->saveFile(fileName,reply->readAll());
        }
    }
}
