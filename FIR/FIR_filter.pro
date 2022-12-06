QT += core gui
QT += printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
CONFIG += console


QMAKE_CXXFLAGS_RELEASE= "-march=native"


SOURCES += \
    main.cpp \
    mainwindow.cpp \
    qcustomplot.cpp \


HEADERS += \
    fir_filter.h \
    mainwindow.h \
    qcustomplot.h \

FORMS += \
    mainwindow.ui


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    vcl/LICENSE \
    vcl/README.md \
    vcl/changelog.txt
