#ifndef COLORMODEL_H
#define COLORMODEL_H

#include <QtCore>

#include "interfaces.h"

// Важная штука: не вызывать handleObject!
class ColorModel: public ShaderGenerator, public Serializable
{
public:
    ColorModel();

    void makeShaders(QGLShaderProgram *program);
    virtual void makeShaders(const QString& itemName, QGLShaderProgram *program) = 0;

    static ColorModel* createDiffuse(const QString& name, Reader *reader);
    static ColorModel* createSpecular(const QString& name, Reader *reader);
};

#endif // COLORMODEL_H
