#ifndef WEBAPI_H
#define WEBAPI_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

class WebAPI : public QObject
{
    Q_OBJECT
public:
    explicit WebAPI(QObject *parent = nullptr);

signals:

private:
    void saveFile(QString fileName, QByteArray content);
    void downloadApk(QUrl url);

public:
    void getApk();
    void getConfig();
//    void getAction();

public slots:
    void slotReponseGettingApk(QNetworkReply* );
    void slotReponseDownloadingApk(QNetworkReply* );
};

#endif // WEBAPI_H
