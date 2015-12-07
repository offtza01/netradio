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

class API : public QObject
{
    Q_OBJECT
public:
    explicit API(QObject *parent = 0);
    void checkVersion();
    void updateLibrary();
    bool isAvaiable();
    void getLocalVersion();
    void sync();
private:
    QNetworkAccessManager *networkManager;
    QSqlDatabase db;
    static const QString apiAddress;
    int downloadingStatus;
    QString downloadingAction;
signals:

public slots:
    void serviceRequestFinished(QNetworkReply*);

};

#endif // API_H
