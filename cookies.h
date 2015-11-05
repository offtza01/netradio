#ifndef COOKIES_H
#define COOKIES_H

#include <QException>
#include <QNetworkCookie>
#include <QNetworkCookieJar>
#include <QStandardPaths>
#include <QDebug>
#include <QDir>

class Cookies : public QNetworkCookieJar
{
    Q_OBJECT
public:
    explicit Cookies(QObject *parent = 0);
    virtual ~Cookies();
    QList<QNetworkCookie> getAllCookies();

signals:

public slots:

private:
    static const QString _defaultDirectory;
    static const QString _defaultFile;
    QString cookieDirectory;
    QDir *cacheDir;
    int readyState;
    QString cookieContent;
    QList<QNetworkCookie> cookieList;

};

#endif // COOKIES_H
