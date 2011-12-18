#ifndef REFRACT_H
#define REFRACT_H

#include "serialization.h"
#include "shadergenerator.h"
#include "shadercode.h"

class Refract: public ShaderGenerator, Serializable
{
    QString m_id;
    ShaderCode* factor;
    ShaderCode* indexOfRefraction;

    int reflectLimit;
    int refractLimit;
public:
    explicit Refract(const QString &id, Reader* reader);
    ~Refract();

    void serialize(Writer *writer) const;
    void makeShaders(const ShaderEmitter &emitter);
};

#endif // REFRACT_H
