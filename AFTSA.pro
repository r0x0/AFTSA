#-------------------------------------------------
#
# Project created by QtCreator 2013-03-05T21:44:53
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AFTSA
TEMPLATE = app thread
QMAKE_CXXFLAGS += -std=c++11
CONFIG += staticlib


SOURCES += main.cpp\
    mainwindow.cpp \
    core/BaseNod.cpp \
    alg/BaseAlg.cpp \
    core/BaseMap.cpp \
    alg/AlgBFS.cpp \
    core/FilesStructure.cpp \
    core/Bitmap.cpp \
    core/Painter.cpp \
    alg/AlgDFS.cpp \
    alg/AlgDFSP.cpp \
    alg/AlgDijkstra.cpp \
    alg/AlgBestFirst.cpp \
    alg/AlgAStar.cpp \
    core/Monitor.cpp \
    core/MinHeap.cpp

HEADERS  += mainwindow.h \
    include.h \
    alg/BaseAlg.h \
    core/BaseNod.h \
    core/BaseMap.h \
    stdinc.h \
    alg/AlgBFS.h \
    core/FilesStructure.h \
    core/Bitmap.h \
    core/SearchTree.h \
    core/Painter.h \
    alg/AlgDFS.h \
    alg/AlgDFSP.h \
    alg/AlgDijkstra.h \
    alg/AlgBestFirst.h \
    alg/AlgAStar.h \
    core/Monitor.h \
    core/MinHeap.h

FORMS    += mainwindow.ui
