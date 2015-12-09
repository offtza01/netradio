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

    localVersion = getLocalVersion();
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
    downloadingStatus = 0;
    if( statusCode == 200)
    {
        if( downloadingAction == "update")
        {
            downloadingAction = "";
            QJsonDocument jsonResponse = QJsonDocument::fromJson(strReply.toUtf8());
            QJsonObject jsonObject = jsonResponse.object();
            foreach(QJsonValue radio, jsonObject)
            {
                Radiostation r;
                r.setHref(radio.toObject()["href"].toString());
                r.setHtmlUid(radio.toObject()["html_uid"].toString());
                r.setIcon(radio.toObject()["icon"].toString());
                r.setIconFile(radio.toObject()["iconFile"].toString().toUtf8());
                r.setName(radio.toObject()["name"].toString());
                r.setStreamAddress(radio.toObject()["stream_address"].toString());
                r.setUid(radio.toObject()["uid"].toString());
                radiostations.append(r);
                updateCurrentVersion();
                updateCurrentLibrary();
                emit updateReady(radiostations);
            }
        }
        if( downloadingAction == "version")
        {
            downloadingAction = "";
            QJsonDocument jsonResponse = QJsonDocument::fromJson(strReply.toUtf8());
            QJsonObject jsonObject = jsonResponse.object();
            if( !jsonObject["release_date"].isUndefined())
            {
                QDateTime apiVersion =  QDateTime::fromString(jsonObject["release_date"].toString(), "yyyy-MM-ddTHH:mm:ss");
                if( apiVersion != localVersion)
                {
                    vc.setHash(jsonObject["hash"].toString());
                    vc.setIsValid(jsonObject["is_valid"].toInt());
                    vc.setReleaseDate(QDateTime::fromString(jsonObject["release_date"].toString()));
                    vc.setIsSet(true);
                    updateLibrary();
                }
            }
        }
    }

}

void API::updateCurrentLibrary()
{
    if( !db.isOpen()) return;
    QSqlQuery query(db);
    query.exec("DELETE FROM radiostations");

    foreach(Radiostation radio, radiostations)
    {
        query.prepare("INSERT INTO radiostations (href, html_uid, icon, name, stream_address, uid) VALUES (:href, :html_uid, :icon, :name, :stream_address, :uid)");
        query.bindValue(":href", radio.getHref());
        query.bindValue(":html_uid", radio.getHtmlUid());
        query.bindValue(":icon", radio.getIcon());
        query.bindValue(":name", radio.getName());
        query.bindValue(":stream_address", radio.getStreamAddress());
        query.bindValue(":uid", radio.getUid());
        query.exec();
        QFile file(radio.getIcon());
        file.open(QIODevice::WriteOnly);
        QDataStream out(&file);
        out << radio.getIconFile();
        file.close();

    }
}


void API::updateCurrentVersion()
{
    if( !vc.getIsSet()) return;
    if( !db.isOpen()) return;
    QSqlQuery query(db);
    query.prepare("INSERT INTO current_version (hash, release_date) VALUES (:hash, :release_date)");
    query.bindValue(":hash", vc.getHash());
    query.bindValue(":release_date", vc.getReleaseDate());
    query.exec();
}

QDateTime API::getLocalVersion()
{
    if( !db.isOpen()) return QDateTime::currentDateTime();

    QSqlQuery query(db);
    query.exec("SELECT * FROM current_version ORDER BY idcurrent_version DESC LIMIT 1");
    if( query.next())
    {
        qDebug() << query.value(0).toString();
        return QDateTime::fromString(query.value(0).toString());
    }
    else
    {
        return QDateTime::currentDateTime();
    }
}

void API::sync()
{
    checkVersion();
}

void API::checkVersion()
{
    if( downloadingStatus == 1) return;
    QUrl url(API::apiAddress + "/version");
    QNetworkRequest request;
    downloadingStatus = 1;
    downloadingAction = (QString)"version";
    request.setUrl(url);
    networkManager->get(request);
}

void API::updateLibrary()
{
    if( downloadingStatus == 1) return;
    QUrl url(API::apiAddress + "/radiostations");
    QNetworkRequest request;
    request.setUrl(url);
    downloadingStatus = 1;
    downloadingAction = (QString)"update";
    networkManager->get(request);
}

QList<Radiostation> API::getRadiostations()
{
    return radiostations;
}
