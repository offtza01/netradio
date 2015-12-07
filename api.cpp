#include "api.h"

const QString API::apiAddress = "http://netradio.offtza.pl";

API::API(QObject *parent) :
    QObject(parent)
{
    networkManager = new QNetworkAccessManager(this);
    downloadingStatus = 0;
    downloadingAction = ' ';

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("radiostations.sq3");
    db.open();


    connect(networkManager, SIGNAL(finished(QNetworkReply*)), this,
            SLOT(serviceRequestFinished(QNetworkReply*)));
}

bool API::isAvaiable()
{
    return db.isOpen();
}

void API::serviceRequestFinished(QNetworkReply *networkReply)
{
    QJsonObject s;
    s.insert("status", QJsonValue(QString("ERROR")));
    QString strReply = (QString)networkReply->readAll();
    int statusCode = networkReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if( statusCode == 200)
    {
        qDebug() << downloadingAction;
        if( downloadingAction == "update")
        {
            qDebug() << "UPDATE";
            QJsonDocument jsonResponse = QJsonDocument::fromJson(strReply.toUtf8());
            QJsonObject jsonObject = jsonResponse.object();
            qDebug() << jsonObject["0"].toObject();
        }
        if( downloadingAction == "version")
        {

            qDebug() << "UPDATE";
            qDebug() << strReply;
        }
        downloadingStatus = 0;
    }

}

void API::getLocalVersion()
{
    if( !db.isOpen()) return;

    QSqlQuery query(db);
    query.exec("SELECT * FROM current_version ORDER BY idcurrent_version DESC LIMIT 1");
    if( query.next())
    {
        qDebug() << query.value(0);
    }
    else
    {
        return 0;
    }
}

void API::sync()
{
    getLocalVersion();
}

void API::checkVersion()
{
    if( downloadingStatus == 1) return;
    QUrl url(API::apiAddress + "/version");
    QNetworkRequest request;
    downloadingStatus = 1;
    downloadingAction = (QString)"version";
    request.setUrl(url);
    QNetworkReply *currentReply = networkManager->get(request);
}

void API::updateLibrary()
{
    if( downloadingStatus == 1) return;
    QUrl url(API::apiAddress + "/radiostations");
    QNetworkRequest request;
    request.setUrl(url);
    downloadingStatus = 1;
    downloadingAction = (QString)"update";
    QNetworkReply *currentReply = networkManager->get(request);
}
