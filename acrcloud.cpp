#include "acrcloud.h"

const QString ACRCloud::account_access_key = "9d1da76a80fbd5b98a1feded565eefdc";
const QString ACRCloud::account_access_secret = "ImseawxlhKkayRRYy8ucoYqhU9Jbx4F7xCzTqrT9";

ACRCloud::ACRCloud(QObject *parent) :
    QObject(parent)
{
    networkManager = new QNetworkAccessManager(this);
    connect(networkManager, SIGNAL(finished(QNetworkReply*)), this,
            SLOT(serviceRequestFinished(QNetworkReply*)));
}

void ACRCloud::serviceRequestFinished(QNetworkReply *networkReply)
{
    QByteArray bytes = networkReply->readAll();
    QString str = QString::fromUtf8(bytes.data(), bytes.size());
    int statusCode = networkReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    qDebug() << statusCode << str;

}

void ACRCloud::loadSettings(QString configurationFilePath)
{
    QSettings settings(configurationFilePath, QSettings::IniFormat);
    reqUrl.setUrl(settings.value("request", "http://ap-southeast-1.api.acrcloud.com/v1/identify").toString());
    method = settings.value("method", "POST").toString();
    uri = settings.value("uri", "/v1/identify").toString();
    signatureVersion = settings.value("signature_version", 1).toString();
    dataType = settings.value("data_type", "audio").toString();

    request = QNetworkRequest(reqUrl);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
}

int ACRCloud::sendRequest(QString mp3FilePath)
{
    //QDateTime timestamp = QDateTime::currentDateTime();
    QDateTime timestamp = QDateTime::fromMSecsSinceEpoch(1436220000);
    qDebug() << (int)timestamp.isValid();
    qDebug() << QString::number(timestamp.toMSecsSinceEpoch());
    qDebug() << timestamp.toMSecsSinceEpoch() << "\n\n\n";
    //return -1;
    QString stringToSign = method + "\n"
            + uri + "\n"
            + account_access_key + "\n"
            + dataType + "\n"
            + signatureVersion + "\n"
            + QString::number(timestamp.toMSecsSinceEpoch());
    qDebug() << stringToSign << "\n";
    QString signature = Crypto::hmacSha1(account_access_key.toUtf8(), stringToSign.toUtf8());
    QFile *mp3FileName = new QFile(mp3FilePath);

    if( !mp3FileName->exists()) return 1;
    if( !mp3FileName->open(QIODevice::ReadOnly)) return 2;

    QString fileContent;
    QTextStream ts(&fileContent, QIODevice::ReadOnly);
    fileContent.append(ts.readAll());

    QUrlQuery postData;
    postData.addQueryItem("data_type", dataType);
    postData.addQueryItem("signature_version", signatureVersion);
    postData.addQueryItem("access_key", account_access_key);
    postData.addQueryItem("sample", fileContent.toUtf8().toBase64());
    postData.addQueryItem("sample_bytes", QString::number(mp3FileName->size()));
    postData.addQueryItem("signature", signature.replace('+', "%2B").toUtf8());
    postData.addQueryItem("timestamp", QString::number(timestamp.toMSecsSinceEpoch()));

    qDebug() << signature;
    qDebug() << postData.hasQueryItem("sample");
    qDebug() << postData.query(QUrl::EncodeUnicode);

    //networkManager->post(request, postData.query(QUrl::FullyEncoded).toUtf8());


    //qDebug() << headerFields.query(QUrl::FullyEncoded).toUtf8();
    return 0;
}
