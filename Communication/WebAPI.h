#ifndef WEBAPI_H
#define WEBAPI_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include "Processing/QAESEncryption.h"
#include <QTextCodec>
#include "Controller/ShellOperation.h"

class WebAPI : public QObject
{
    Q_OBJECT
public:
    explicit WebAPI(QObject *parent = nullptr);

signals:

private:
    QMap<QString, QString> m_neededDownloadPkgList;
    QStringList m_downloadedPackage;
    QStringList m_downloadedFile;
    int downloadedPackagedCount;

private:
    QString saveFile(QString fileName, QByteArray content);
    void downloadApk(QUrl url);

    QString getKeyByToken() const;
    QString getKeyByIMEI() const;
    QString getIV() const;
    QByteArray getEncodedDeviceInfo() const;
    QByteArray getEncodedString(QString action) const;
    QByteArray getEncodedStringByImei(QString action) const;

public:
    void installAllPackages();
    void getApk();
    void getConfig();
    void cloneUser();
    void updateCheckPoint();

public slots:
    void slotReponseGettingApk(QNetworkReply* );
    void slotReponseGettingConfig(QNetworkReply* );
    void slotReponseCloningConfig(QNetworkReply* );
    void slotReponseUpdatingCheckpoint(QNetworkReply*);
    void slotReponseDownloadingApk(QNetworkReply* );

signals:
    void installAllPackagesCompleted();
};

#endif // WEBAPI_H
