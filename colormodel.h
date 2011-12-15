#ifndef COLORMODEL_H
#define COLORMODEL_H

#include <QtCore>

#include "interfaces.h"

// Важная штука: не вызывать handleObject!
class ColorModel: public ShaderGenerator, public Serializable
{
public:
    static ColorModel* createDiffuse(const QString& name, Reader *reader);
    static ColorModel* createSpecular(const QString& name, Reader *reader);
};

#endif // COLORMODEL_H
