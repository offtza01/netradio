#ifndef RADIOSTATION_H
#define RADIOSTATION_H

#include <QString>
#include <QSignalMapper>
#include <QDateTime>

class Radiostation
{
public:
    QString getHref() const;
    void setHref(const QString &value);

    QString getHtmlUid() const;
    void setHtmlUid(const QString &value);

    QString getIcon() const;
    void setIcon(const QString &value);

    QByteArray getIconFile() const;
    void setIconFile(const QByteArray &value);

    QString getName() const;
    void setName(const QString &value);

    QString getStreamAddress() const;
    void setStreamAddress(const QString &value);

    QString getUid() const;
    void setUid(const QString &value);

private:
    QString href;
    QString html_uid;
    QString icon;
    QByteArray iconFile;
    QString name;
    QString stream_address;
    QString uid;
signals:

public slots:

};


class VersionControl
{
public:
    explicit VersionControl();
    QString getHash() const;
    void setHash(const QString &value);

    QDateTime getReleaseDate() const;
    void setReleaseDate(const QDateTime &value);

    int getIsValid() const;
    void setIsValid(int value);

    bool getIsSet() const;
    void setIsSet(bool value);

private:
    QString hash;
    QDateTime release_date;
    int isValid;
    bool isSet;
};

#endif // RADIOSTATION_H
