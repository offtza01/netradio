#ifndef ACRCLOUD_H
#define ACRCLOUD_H

#include <QObject>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QSettings>
#include <QTime>
#include <QUrlQuery>

class ACRCloud : public QObject
{
    Q_OBJECT
public:
    explicit ACRCloud(QObject *parent = 0);
    void loadSettings(QString fileName="netradio.conf");
    QString reqUrl;
    QString method;
    QString uri;
    QString dataType;
    int signatureVersion;
    QTime timestamp;
    QString stringToSign;
    QString signature;
    QString mp3FileName;
    int mp3FileSize;
    QString content;
    QUrlQuery postFields;

signals:

public slots:

};

#endif // ACRCLOUD_H
