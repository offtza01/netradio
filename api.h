#ifndef API_H
#define API_H

#include <QObject>
#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include <QtSql/QtSql>
#include <QDateTime>

#include <radiostation.h>

class API : public QObject
{
    Q_OBJECT
public:
    explicit API(QObject *parent = 0);
    void checkVersion();
    void updateLibrary();
    bool isAvaiable();
    QDateTime getLocalVersion();
    void sync();
    QList<Radiostation> getRadiostations();
private:
    QNetworkAccessManager *networkManager;
    QSqlDatabase db;
    static const QString apiAddress;
    int downloadingStatus;
    QString downloadingAction;
    QDateTime localVersion;
    QList<Radiostation> radiostations;
    VersionControl vc;
    void updateCurrentLibrary();
    void updateCurrentVersion();
signals:
    void updateReady(QList<Radiostation>);
public slots:
    void serviceRequestFinished(QNetworkReply*);

};

#endif // API_H
