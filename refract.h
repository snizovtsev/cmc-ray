#ifndef REFRACT_H
#define REFRACT_H

#include "serialization.h"
#include "shadergenerator.h"
#include "shadercode.h"

class Refract: public ShaderGenerator, public Serializable
{
    QString m_id;
    ShaderCode* m_factor;
    ShaderCode* indexOfRefraction;

    int reflectLimit;
    int refractLimit;
public:
    explicit Refract(const QString &id, Reader* reader);
    ~Refract();

    ShaderCode* factor() const { return m_factor; }

    void serialize(Writer *writer) const;
    void makeShaders(const ShaderEmitter &emitter);
};

#endif // REFRACT_H
