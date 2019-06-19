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

    json.insert("action", QTextCodec::codecForMib(106)->toUnicode(getEncodedAction("getapk")));
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

QByteArray WebAPI::getEncodedAction(QString action) const
{
    QAESEncryption encryption(QAESEncryption::AES_256, QAESEncryption::CBC);
    LOG << encryption.encode(action.toLocal8Bit(), getKeyByToken().toLocal8Bit(), getIV().toLocal8Bit()).toBase64();
    return encryption.encode(action.toLocal8Bit(), getKeyByToken().toLocal8Bit(), getIV().toLocal8Bit()).toBase64();
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

    if(jsonObj["action"].toString() == QTextCodec::codecForMib(106)->toUnicode(getEncodedAction("getapk"))){
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
