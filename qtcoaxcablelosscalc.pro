QT       += core gui widgets charts printsupport

TARGET = qtcoaxcablelosscalc

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += $$PWD/3rdparty/qcustomplot
INCLUDEPATH += $$PWD/3rdparty/spline/src

SOURCES += \
    3rdparty/qcustomplot/qcustomplot.cpp \
    cablemodel.cpp \
    cablewidget.cpp \
    main.cpp \
    mainwindow.cpp \
    qtcoaxcablelosscalcmanager.cpp

HEADERS += \
    3rdparty/qcustomplot/qcustomplot.h \
    3rdparty/spline/src/spline.h \
    cablemodel.h \
    cablewidget.h \
    mainwindow.h \
    qtcoaxcablelosscalcmanager.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
