#ifndef CRYPTO_H
#define CRYPTO_H

#include <QObject>
#include <QDebug>
#include <QCryptographicHash>

class Crypto : public QObject
{
    Q_OBJECT
public:
    explicit Crypto(QObject *parent = 0);
    static QString hmacSha1(QByteArray key, QByteArray baseString);

signals:

public slots:

};

#endif // CRYPTO_H
