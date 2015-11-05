#ifndef PLAYER_H
#define PLAYER_H

#include <QGraphicsWebView>
#include <QWebFrame>
#include <QEventLoop>
#include <QMap>
#include <QWebView>
#include <QTimer>
#include <QTime>
#include <QDir>
#include "bass.h"
#include "html5applicationviewer/html5applicationviewer.h"
#include "acrcloud.h"

class Player : public Html5ApplicationViewer
{
    Q_OBJECT
public:
    explicit Player(QWidget *parent = 0);
    static FILE *recordFileH;
    static QString recordFilePath;
    static bool isRecording;
    static const int recordTime;

private:
    HSTREAM stream;
    HRECORD rstream;
    bool playStatus;
    QTimer *timer;
    QMap<QString, QString> playList;
    static void StatusProc(const void *buffer, DWORD length, void *user);
    static void MyDownload(const void *buffer, DWORD length, void *user);
    ACRCloud acr;

private slots:
    void addToJavaScript();
    bool PrebufTimerProc();
    void stopRecording();
    void songFound(QJsonObject song);

public slots:
    void play(QString stationId="unknown");
    void setVolume(float volume);
    float getValue();
    void stop();
    void record();
};

#endif // PLAYER_H
