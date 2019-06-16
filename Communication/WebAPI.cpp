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
    QString url = API_SERVER + QString("getapk?token=%1").arg(MODEL->token());
    LOG << "url: " << url;
    QUrl serviceUrl = QUrl(url);
    QNetworkRequest request(serviceUrl);
    QJsonObject json;
    QJsonObject deviceInfo;

    deviceInfo["GoogleSF"] = "31b75eb8d";
    deviceInfo["AndroidID"] = "391f3";
    deviceInfo["IMEI"] = "35697865";
    deviceInfo["IMSI"] = "456789987776";
    deviceInfo["SIMCardSerial"] = "89876656";
    deviceInfo["WifiMacAddress"] = "24:26:38";
    deviceInfo["android_verion"] = "7.1.1";
    deviceInfo["model"] = "Sony Z3";
    json.insert("action","getapk");
    json.insert("device",deviceInfo);

    QJsonDocument jsonDoc(json);
    LOG << "jsonDoc: " << jsonDoc;
    QByteArray jsonData= jsonDoc.toJson();
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

void WebAPI::slotReponseGettingApk(QNetworkReply* reply)
{
    QByteArray responseData = reply->readAll();
    QJsonDocument jdoc = QJsonDocument::fromJson(responseData);

    foreach (QJsonValue data, jdoc.array()) {
        if(data.isObject()){
            QJsonObject obj = data.toObject();
            this->downloadApk(QUrl(obj["apk"].toString()));
        }
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
