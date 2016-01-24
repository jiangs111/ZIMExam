#-------------------------------------------------
#
# Project created by QtCreator 2015-11-27T13:57:32
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QT       += sql widgets
QT       += axcontainer
QT       += concurrent
QT       += printsupport

TARGET = ZimexamServer
TEMPLATE = app


SOURCES += main.cpp\
        loginserver.cpp \
    manageform.cpp \
    helpform.cpp \
    assistant.cpp \
    passwordform.cpp \
    studentform.cpp \
    importexcel.cpp \
    questionform.cpp \
    importquestion.cpp \
    paperbank.cpp

HEADERS  += loginserver.h \
    manageform.h \
    helpform.h \
    assistant.h \
    passwordform.h \
    studentform.h \
    importexcel.h \
    questionform.h \
    importquestion.h \
    paperbank.h

FORMS    += loginserver.ui \
    manageform.ui \
    helpform.ui \
    passwordform.ui \
    studentform.ui \
    importexcel.ui \
    questionform.ui \
    importquestion.ui \
    paperbank.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/poppler-0.24.5-win32/lib/ -llibpoppler-qt5
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/poppler-0.24.5-win32/lib/ -llibpoppler-qt5
else:unix: LIBS += -L$$PWD/poppler-0.24.5-win32/lib/ -llibpoppler-qt5

INCLUDEPATH += $$PWD/poppler-0.24.5-win32/include/poppler-qt5
DEPENDPATH += $$PWD/poppler-0.24.5-win32/include/poppler-qt5
