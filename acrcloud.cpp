#include "acrcloud.h"

ACRCloud::ACRCloud(QObject *parent) :
    QObject(parent)
{

}

void ACRCloud::loadSettings(QString fileName)
{
    QSettings settings(fileName, QSettings::IniFormat);
    reqUrl = settings.value("request", "http://ap-southeast-1.api.acrcloud.com/v1/identify").toString();
    uri = settings.value("uri", "/v1/identify").toString();
    method = settings.value("method", "POST").toString();
    dataType = settings.value("data_type", "audio").toString();
    postFields.addQueryItem("data_type", dataType);
    qDebug() << postFields.query(QUrl::FullyEncoded).toUtf8();
}
