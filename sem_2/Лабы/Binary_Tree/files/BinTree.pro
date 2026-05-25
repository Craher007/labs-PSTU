QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
TARGET = BinTree
TEMPLATE = app

SOURCES += \
    BinaryTree.cpp \
    TreeWidget.cpp \
    Mainwindow.cpp \
    main.cpp

HEADERS += \
    BinaryTree.h \
    Node.h \
    TreeWidget.h \
    Mainwindow.h
