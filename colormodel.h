#ifndef COLORMODEL_H
#define COLORMODEL_H

#include <QtCore>

#include "serialization.h"
#include "shadergenerator.h"

// Важная штука: не вызывать handleObject!
class ColorModel: public ShaderGenerator, public Serializable
{
public:
    ColorModel();

    void makeShaders(const ShaderEmitter &emitter);
    virtual void makeShaders(const QString& itemName, const ShaderEmitter &emitter) = 0;

    static ColorModel* createDiffuse(const QString& name, Reader *reader);
    static ColorModel* createSpecular(const QString& name, Reader *reader);
};

#endif // COLORMODEL_H
