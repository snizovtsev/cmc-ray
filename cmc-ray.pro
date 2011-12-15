#-------------------------------------------------
#
# Project created by QtCreator 2011-11-22T18:48:07
#
#-------------------------------------------------

QT += core gui opengl

TARGET = cmc-ray
TEMPLATE = app

SOURCES += main.cpp\
    widget.cpp \
    scene.cpp \
    shadercode.cpp \
    material.cpp \
    item.cpp \
    fog.cpp \
    colormodel.cpp

HEADERS  += widget.h \
    scene.h \
    interfaces.h \
    shadercode.h \
    material.h \
    colormodel.h \
    item.h \
    fog.h

OTHER_FILES += scene.xml \
    main.vert \
    fog.frag \
    scene.frag

RESOURCES += \
    resources.qrc
