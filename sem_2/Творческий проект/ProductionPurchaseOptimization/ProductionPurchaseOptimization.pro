QT += widgets

CONFIG += c++17
CONFIG -= app_bundle

TARGET = ProductionPurchaseOptimizationQt
TEMPLATE = app

SOURCES += \
    src/QtMain.cpp \
    src/MainWindow.cpp \
    src/ItemDialog.cpp \
    src/DateUtils.cpp \
    src/Database.cpp \
    src/Planner.cpp \
    src/Report.cpp

HEADERS += \
    src/MainWindow.h \
    src/ItemDialog.h \
    src/DateUtils.h \
    src/Database.h \
    src/Planner.h \
    src/Report.h \
    src/Models.h
