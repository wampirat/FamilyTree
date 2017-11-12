#-------------------------------------------------
#
# Project created by QtCreator 2016-02-28T14:34:27
#
#-------------------------------------------------

QT += core gui

QT += widgets

QT += printsupport

TARGET = FamilyTree
TEMPLATE = app
RC_FILE = familyTree.rc

INCLUDEPATH += "../FamilyTreeEngine/"

SOURCES += main.cpp\
    ../FamilyTreeEngine/PeopleCollection.cpp \
    ../FamilyTreeEngine/Person.cpp \
    ../FamilyTreeEngine/Family.cpp \
    mainwindow.cpp \
    dialogaddperson.cpp \
    dialogaddfamily.cpp \
    treeview.cpp \
    FamilyTreePainterPathsPreparator.cpp

HEADERS  += \
    ../FamilyTreeEngine/Family.h \
    ../FamilyTreeEngine/FamilyFactory.hpp \
    ../FamilyTreeEngine/FamilyTreeFramework.hpp \
    ../FamilyTreeEngine/PeopleCollection.hpp \
    ../FamilyTreeEngine/Person.hpp \
    ../FamilyTreeEngine/SocialPerson.hpp \
    mainwindow.h \
    dialogaddperson.h \
    dialogaddfamily.h \
    treeview.h \
    FamilyTreePainterPathsPreparator.hpp

CONFIG += c++14

QMAKE_CXXFLAGS += -static-libgcc -static-libstdc++

FORMS += \
    mainwindow.ui \
    dialogaddperson.ui \
    dialogaddfamily.ui \
    treeview.ui

DISTFILES += \
    familyTree.rc \
    icon.ico
