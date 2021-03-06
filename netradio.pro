greaterThan(QT_MAJOR_VERSION, 4):QT += widgets webkitwidgets

# Add more folders to ship with the application, here
folder_01.source = html
folder_01.target = .
DEPLOYMENTFOLDERS = folder_01
INCLUDEPATH += $$PWD/lib
CONSOLE += console
CONFIG += static
QT += network sql

# Define TOUCH_OPTIMIZED_NAVIGATION for touch optimization and flicking
#DEFINES += TOUCH_OPTIMIZED_NAVIGATION

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += main.cpp \
    player.cpp \
    acrcloud.cpp \
    crypto.cpp \
    cookies.cpp \
    api.cpp \
    radiostation.cpp

# Please do not modify the following two lines. Required for deployment.
include(html5applicationviewer/html5applicationviewer.pri)
qtcAddDeployment()

HEADERS += \
    bass.h \
    player.h \
    acrcloud.h \
    crypto.h \
    cookies.h \
    api.h

RESOURCES += \
    resource.qrc


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/lib/release/ -lbass
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/debug/ -lbass
else:unix: LIBS += -L$$PWD/lib/ -lbass

INCLUDEPATH += $$PWD/
DEPENDPATH += $$PWD/
