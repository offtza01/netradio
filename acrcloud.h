#ifndef ACRCLOUD_H
#define ACRCLOUD_H

#include <QObject>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QSettings>
#include <QDateTime>
#include <QTime>
#include <QUrlQuery>
#include <QFile>

#include "crypto.h"
#include "cookies.h"

class ACRCloud : public QObject
{
    Q_OBJECT
public:
    explicit ACRCloud(QObject *parent = 0);
    void loadSettings(QString configurationFilePath="netradio.conf");
    int sendRequest(QString mp3FilePath);

    QUrl reqUrl;
    QString method;
    QString uri;
    QString dataType;
    QString signatureVersion;
private:
    static const QString account_access_key;
    static const QString account_access_secret;
    QNetworkRequest request;
    QNetworkAccessManager *networkManager;
    Cookies *cookieJar;
signals:
    void songFound(QJsonObject song);
public slots:
    void serviceRequestFinished(QNetworkReply*);
};

#endif // ACRCLOUD_H
