#include "acrcloud.h"

const QString ACRCloud::account_access_key = "xxx";
const QString ACRCloud::account_access_secret = "xxx";

ACRCloud::ACRCloud(QObject *parent) :
    QObject(parent)
{
    networkManager = new QNetworkAccessManager(this);
    cookieJar = new Cookies();
    networkManager->setCookieJar(cookieJar);

    connect(networkManager, SIGNAL(finished(QNetworkReply*)), this,
            SLOT(serviceRequestFinished(QNetworkReply*)));
}

void ACRCloud::serviceRequestFinished(QNetworkReply *networkReply)
{
    QJsonObject s;
    s.insert("status", QJsonValue(QString("ERROR")));
    QByteArray bytes = networkReply->readAll();
    QString strReply = QString::fromUtf8(bytes.data(), bytes.size());
    int statusCode = networkReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if( statusCode == 200)
    {
        qDebug() << strReply;
        QJsonDocument jsonResponse = QJsonDocument::fromJson(strReply.toUtf8());
        QJsonObject jsonObject = jsonResponse.object();
        if( jsonObject["status"].toObject()["code"].toInt() == 0)
        {
            QJsonValue music = jsonObject["metadata"].toObject()["music"];
            QJsonArray song = music.toArray();

            s.insert("title", song[0].toObject()["title"].toString());
            s.insert("artist",song[0].toObject()["artists"].toArray()[0].toObject()["name"].toString());
            s.insert("status", QString("OK"));
        }
    }
    qDebug() << networkReply->readAll();
    emit songFound(s);
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
    QDateTime timestamp = QDateTime::fromMSecsSinceEpoch(1436220000);
    QString stringToSign = method + "\n"
            + uri + "\n"
            + account_access_key + "\n"
            + dataType + "\n"
            + signatureVersion + "\n"
            + QString::number(timestamp.toMSecsSinceEpoch());
    stringToSign = stringToSign.replace("+", "%2B");
    QString signature = Crypto::hmacSha1(account_access_secret.toUtf8(), stringToSign.toUtf8());
    qDebug() << "signature";
    qDebug() << signature;

    QFile mp3FileName(mp3FilePath);

    if( !mp3FileName.exists()) return 1;
    if( !mp3FileName.open(QIODevice::ReadOnly)) return 2;

    QByteArray fileContent = mp3FileName.readAll();
    fileContent = fileContent.toBase64();
    QString fContent = QString(fileContent);
    fContent = fContent.replace("+", "%2B");
    mp3FileName.close();

    QUrlQuery postData;
    postData.addQueryItem("data_type", dataType);
    postData.addQueryItem("signature_version", signatureVersion);
    postData.addQueryItem("access_key", account_access_key);
    postData.addQueryItem("sample", fContent);
    postData.addQueryItem("sample_bytes", QString::number(mp3FileName.size()));
    postData.addQueryItem("signature", signature.replace('+', "%2B").toUtf8());
    postData.addQueryItem("timestamp", QString::number(timestamp.toMSecsSinceEpoch()));

    networkManager->post(request, postData.query(QUrl::FullyEncoded).toUtf8());

    return 0;
}
