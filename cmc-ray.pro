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
    colormodel.cpp \
    xmlreader.cpp \
    xmlwriter.cpp \
    serialization.cpp \
    shadergenerator.cpp \
    refract.cpp

HEADERS  += widget.h \
    scene.h \
    serialization.h \
    shadercode.h \
    material.h \
    colormodel.h \
    item.h \
    fog.h \
    xmlreader.h \
    xmlwriter.h \
    shadergenerator.h \
    refract.h

OTHER_FILES += scene.xml \
    main.vert \
    fog.frag \
    scene.frag \
    colormodel.frag \
    item.frag \
    refract.frag

RESOURCES += \
    resources.qrc
