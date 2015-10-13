#include <QApplication>
#include <QFile>
#include "html5applicationviewer.h"
#include "player.h"


#ifdef QT_DEBUG
#include <QDebug>
#include <QWebInspector>
#endif



int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    Player viewer;

    viewer.setOrientation(Html5ApplicationViewer::ScreenOrientationAuto);
    viewer.showExpanded();
    QSize fixedSize(800,500);
    viewer.setMinimumSize(fixedSize);
    viewer.setMaximumSize(fixedSize);
    //viewer.setWindowFlags( Qt::CustomizeWindowHint );
    viewer.setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    #ifdef QT_DEBUG
    qDebug() << "Running a debug build";
    viewer.webView()->page()->settings()->setAttribute(QWebSettings::DeveloperExtrasEnabled, true);
    QWebInspector inspector;
    inspector.setPage(viewer.webView()->page());
    inspector.setVisible(true);
    #endif


    QFile f("html/css/style.css");
    f.open(QFile::ReadOnly);
    QString StyleSheet = QLatin1String(f.readAll());
    viewer.loadFile(QLatin1String("html/index.html"));
    viewer.webView()->setAcceptHoverEvents(true);

    //int x = (screenGeometry.width()-mainWindow->width()) / 2;
    //int y = (screenGeometry.height()-mainWindow->height()) / 2;
    viewer.show();
    return app.exec();
}
