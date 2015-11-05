#include "cookies.h"

const QString Cookies::_defaultDirectory = "/.netradio/";
const QString Cookies::_defaultFile = "session.cache";

Cookies::Cookies(QObject *parent) :
    QNetworkCookieJar(parent)
{
    cookieDirectory = QStandardPaths::standardLocations(QStandardPaths::HomeLocation).at(0) + Cookies::_defaultDirectory;
    cacheDir = new QDir(cookieDirectory);
    readyState = 1;

    if( !cacheDir->exists())
    {
        if( !cacheDir->mkdir(cookieDirectory))
        {
            readyState = 2;
            return;
        }
    }
    QFile f(cookieDirectory + Cookies::_defaultFile);
    if( f.exists())
    {
        if (!f.open(QFile::ReadOnly | QFile::Text))
        {
            readyState = 3;
            return;
        }
        QTextStream in(&f);
        cookieContent = in.readAll();
        f.close();
    }

    readyState = 0;
}

Cookies::~Cookies()
{
    qDebug() << "zabijam";
    cookieList = allCookies();
    qDebug() << cookieList.isEmpty();
    return;
    QFile f(cookieDirectory + Cookies::_defaultFile);
    if( f.exists())
    {
        f.remove();
    }

    if( f.open(QFile::WriteOnly | QFile::Text))
    {
        QTextStream out(&f);

        //out << allCookies();
        f.close();
    }
}

QList<QNetworkCookie> Cookies::getAllCookies()
{
    return allCookies();
}
