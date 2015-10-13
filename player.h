#ifndef PLAYER_H
#define PLAYER_H

#include <QGraphicsWebView>
#include <QWebFrame>
#include <QEventLoop>
#include <QMap>
#include <QWebView>
#include "bass.h"
#include "html5applicationviewer/html5applicationviewer.h"

class Player : public Html5ApplicationViewer
{
    Q_OBJECT
public:
    explicit Player(QWidget *parent = 0);
private:
    HSTREAM stream;
    bool playStatus;
    QTimer *timer;
    QMap<QString, QString> playList;
    static void StatusProc(const void *buffer, DWORD length, void *user);

private slots:
    void addToJavaScript();
    bool PrebufTimerProc();

public slots:
    void play(QString stationId="unknown");
    void setVolume(float volume);
    void stop();
};

#endif // PLAYER_H
