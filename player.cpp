#include "player.h"

FILE * Player::recordFileH = NULL;
QString Player::recordFilePath = NULL;
const int Player::recordTime = 10;
bool Player::isRecording = false;



Player::Player(QWidget *parent) :Html5ApplicationViewer(parent) {
    QObject::connect(webView()->page()->mainFrame(),
            SIGNAL(javaScriptWindowObjectCleared()), SLOT(addToJavaScript()));

    playStatus = false;
    if (HIWORD(BASS_GetVersion())!=BASSVERSION) {
            return;
    }

    if (!BASS_Init(-1,44100,0,NULL,NULL)) {
            return;
    }
    BASS_SetConfig(BASS_CONFIG_NET_PLAYLIST,1); // enable playlist processing
    BASS_SetConfig(BASS_CONFIG_NET_PREBUF,0); // minimize automatic pre-buffering, so we can do it (and display it) instead
    playList.insert("1", "http://mp3.polskieradio.pl:8900/;stream");
    playList.insert("2", "http://stream3.polskieradio.pl:8902/;stream");
    playList.insert("3", "http://stream3.polskieradio.pl:8904/;stream");
    playList.insert("4", "http://mp3.polskieradio.pl:8906/;stream");
    playList.insert("antyradio", "http://ant-waw.cdn.eurozet.pl:8602/;stream");
    acr.loadSettings();
    QObject::connect(&acr, SIGNAL(songFound(QJsonObject)),
                     this, SLOT(songFound(QJsonObject)));
}

void Player::songFound(QJsonObject song)
{
    QJsonDocument doc(song);
    QByteArray bytes = doc.toJson();

    if( song.value("status").toString() == "OK")
    {
        webView()->page()->mainFrame()->evaluateJavaScript(QString("setRecordNotice(\"Utwór rozpoznano!\")"));
        webView()->page()->mainFrame()->evaluateJavaScript(QString("enableYT("+ QString::fromUtf8(bytes) +")"));
    }
    else
    {
        webView()->page()->mainFrame()->evaluateJavaScript(QString("setRecordNotice(\"Utwór nie został rozpoznany :(\")"));
    }
    webView()->page()->mainFrame()->evaluateJavaScript(QString("enablePlaylistClicking()"));
}


void Player::addToJavaScript() {
    webView()->page()->mainFrame()->addToJavaScriptWindowObject("Player", this);
}

bool Player::PrebufTimerProc()
{
        QWORD progress = BASS_StreamGetFilePosition(stream,BASS_FILEPOS_BUFFER)*100/BASS_StreamGetFilePosition(stream,BASS_FILEPOS_END);

        if (progress>75 || !BASS_StreamGetFilePosition(stream,BASS_FILEPOS_CONNECTED)) { // over 75% full (or end of download)
                { // get the broadcast name and URL
                        const char *icy=BASS_ChannelGetTags(stream,BASS_TAG_ICY);
                        if (!icy) icy=BASS_ChannelGetTags(stream,BASS_TAG_HTTP); // no ICY tags, try HTTP
                        if (icy) {

                        }
                }
                // get the stream title and set sync for subsequent titles
                //DoMeta();
                //BASS_ChannelSetSync(chan,BASS_SYNC_META,0,&MetaSync,0); // Shoutcast
                //BASS_ChannelSetSync(chan,BASS_SYNC_OGG_CHANGE,0,&MetaSync,0); // Icecast/OGG
                // set sync for end of stream
                //BASS_ChannelSetSync(chan,BASS_SYNC_END,0,&EndSync,0);
                // play it!
                BASS_ChannelPlay(stream,FALSE);

                return FALSE; // stop monitoring
        } else {
                return TRUE; // continue monitoring
        }
}

void CALLBACK Player::MyDownload(const void *buffer, DWORD length, void *user)
{
    if( isRecording == false) return;
    if (!recordFileH){
        recordFilePath = QDir::tempPath() + "/_netradio_" + QString::number(qrand());
        qDebug() << recordFilePath;
        recordFileH = fopen(recordFilePath.toStdString().c_str(), "wb");
    }
    if (!buffer){
        fclose(recordFileH); // finished downloading
    }
    else
    {
        fwrite(buffer, 1, length, recordFileH);
    }
}


void CALLBACK Player::StatusProc(const void *buffer, DWORD length, void *user)
{
        if (buffer && !length ) { // got HTTP/ICY tags, and this is still the current request
            qDebug() << buffer;
        }
}

void Player::play(QString stationId) {

    stop();
    if( !playList.contains(stationId))
    {
        return;
        //return QString("Station not found");
    }

    HSTREAM stream = BASS_StreamCreateURL(playList.value(stationId).toStdString().c_str(),0,BASS_STREAM_BLOCK|BASS_STREAM_STATUS|BASS_STREAM_AUTOFREE,MyDownload,0);

    BASS_Start();

    if( stream)
    {
        bool buffering = true;
        while(buffering)
        {
            QWORD progress = BASS_StreamGetFilePosition(stream,BASS_FILEPOS_BUFFER)*100/BASS_StreamGetFilePosition(stream,BASS_FILEPOS_END);
            webView()->page()->mainFrame()->evaluateJavaScript(QString("PlayerHud.updateBufforStatus("+ QString::number(progress) +")"));
            if( progress>=100)
            {
                buffering = false;
                webView()->page()->mainFrame()->evaluateJavaScript("PlayerHud.clearBufforStatus()");
            }

        }
        BASS_ChannelPlay(stream,FALSE);
        playStatus = true;

    }
    else
    {
        qDebug() << BASS_ErrorGetCode();
        return;
    }
    return;
}

void Player::stop() {
    if( playStatus == true)
    {
        BASS_Stop();
        BASS_StreamFree(stream);
        playStatus = false;
    }
}

void Player::setVolume(float volume)
{
    BASS_SetVolume(volume);
}

float Player::getValue()
{
    return BASS_GetVolume();
}


void Player::record()
{
    stopRecording();
    QTimer::singleShot(recordTime*1000, this, SLOT(stopRecording()));
    isRecording = true;
}

void Player::stopRecording()
{
    if( isRecording == false) return;
    isRecording = false;
    if( recordFileH != NULL)
    {
        fclose(recordFileH);
        webView()->page()->mainFrame()->evaluateJavaScript(QString("setRecordNotice(\"Wysyłanie zapytania...\")"));
        acr.sendRequest(recordFilePath);
        recordFileH = NULL;
    }
}
