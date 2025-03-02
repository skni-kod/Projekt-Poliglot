QT       += core gui
QT += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/board.cpp \
    src/completiondialog.cpp \
    src/customdialog.cpp \
    src/gridcell.cpp \
    src/imageservice.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/user.cpp \
    src/word_gen.cpp

HEADERS += \
    src/board.h \
    src/completiondialog.h \
    src/customdialog.h \
    src/gridcell.h \
    src/imageservice.h \
    src/mainwindow.h \
    src/user.h \
    src/word_gen.h

FORMS += \
    ui/mainwindow.ui

RESOURCES += \
    images.qrc \
    user.qrc \
    wordBase.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


