QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

RC_ICONS = icon.ico

SOURCES += \
    checkboxwidget.cpp \
    clickablelabel.cpp \
    comboboxwidget.cpp \
    filesdialog.cpp \
    labelwidget.cpp \
    lineeditwidget.cpp \
    main.cpp \
    mainwindow.cpp \
    mywidget.cpp \
    plaintextwidget.cpp \
    radiobuttonwidget.cpp \
    spinboxwidget.cpp

HEADERS += \
    checkboxwidget.h \
    clickablelabel.h \
    comboboxwidget.h \
    filesdialog.h \
    labelwidget.h \
    lineeditwidget.h \
    mainwindow.h \
    mywidget.h \
    plaintextwidget.h \
    radiobuttonwidget.h \
    spinboxwidget.h

FORMS += \
    filesdialog.ui \
    mainwindow.ui \
    test.ui

TRANSLATIONS += \
    StreamAssistant_pt_BR.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    StreamAssistant.pro.user


HEADERS += \
    checkboxwidget.h \
    comboboxwidget.h \
    filesdialog.h \
    labelwidget.h \
    lineeditwidget.h \
    mainwindow.h \
    mywidget.h \
    plaintextwidget.h \
    radiobuttonwidget.h \
    spinboxwidget.h

FORMS += \
    filesdialog.ui \
    mainwindow.ui \
    test.ui

TRANSLATIONS += \
    StreamAssistant_pt_BR.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    StreamAssistant.pro.user
