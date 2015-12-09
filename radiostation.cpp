#include "radiostation.h"

QString Radiostation::getHref() const
{
    return href;
}

void Radiostation::setHref(const QString &value)
{
    href = value;
}
QString Radiostation::getHtmlUid() const
{
    return html_uid;
}

void Radiostation::setHtmlUid(const QString &value)
{
    html_uid = value;
}
QString Radiostation::getIcon() const
{
    return icon;
}

void Radiostation::setIcon(const QString &value)
{
    icon = value;
}
QByteArray Radiostation::getIconFile() const
{
    return iconFile;
}

void Radiostation::setIconFile(const QByteArray &value)
{
    iconFile = value;
}
QString Radiostation::getName() const
{
    return name;
}

void Radiostation::setName(const QString &value)
{
    name = value;
}
QString Radiostation::getStreamAddress() const
{
    return stream_address;
}

void Radiostation::setStreamAddress(const QString &value)
{
    stream_address = value;
}
QString Radiostation::getUid() const
{
    return uid;
}

void Radiostation::setUid(const QString &value)
{
    uid = value;
}


VersionControl::VersionControl()
{
    isSet = false;
}

QString VersionControl::getHash() const
{
    return hash;
}

void VersionControl::setHash(const QString &value)
{
    hash = value;
}
QDateTime VersionControl::getReleaseDate() const
{
    return release_date;
}

void VersionControl::setReleaseDate(const QDateTime &value)
{
    release_date = value;
}
int VersionControl::getIsValid() const
{
    return isValid;
}

void VersionControl::setIsValid(int value)
{
    isValid = value;
}
bool VersionControl::getIsSet() const
{
    return isSet;
}

void VersionControl::setIsSet(bool value)
{
    isSet = value;
}



