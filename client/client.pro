QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    authform.cpp \
    client.cpp \
    communication.cpp \
    contest.cpp \
    curve.cpp \
    formcontests.cpp \
    formdetailinfo.cpp \
    formnoconnection.cpp \
    formregcontest.cpp \
    horse.cpp \
    jockey.cpp \
    kuznechik.cpp \
    main.cpp \
    mainform.cpp \
    mqv.cpp \
    place.cpp

HEADERS += \
    authform.h \
    client.h \
    communication.h \
    contest.h \
    curve.h \
    formcontests.h \
    formdetailinfo.h \
    formnoconnection.h \
    formregcontest.h \
    horse.h \
    jockey.h \
    kuznechik.h \
    mainform.h \
    mqv.h \
    place.h

FORMS += \
    authform.ui \
    formcontests.ui \
    formdetailinfo.ui \
    formnoconnection.ui \
    formregcontest.ui \
    mainform.ui


LIBS += \
        -lboost_system\

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
