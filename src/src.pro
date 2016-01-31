TEMPLATE:unix = app
TEMPLATE:win32 = vcapp
CONFIG += warn_on \
    thread \
    qt \
    debug
QT += core widgets sql
win32:debug:CONFIG += console
TARGET = ../bin/qbudget2
RESOURCES = application.qrc
FORMS += topic_dialog.ui \
    mainwindow.ui
HEADERS += transaction_editor.h \
    mainwindow.h \
    db_manager.h \
    topic_dialog.h \
    transaction_model.h \
    transaction.h \
    topic.h \
    cdate.h
SOURCES += transaction_editor.cpp \
    main.cpp \
    mainwindow.cpp \
    db_manager.cpp \
    topic_dialog.cpp \
    transaction_model.cpp \
    transaction.cpp \
    topic.cpp \
    cdate.cpp
