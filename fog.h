#ifndef FOG_H
#define FOG_H

#include "serialization.h"
#include "shadergenerator.h"
#include "shadercode.h"

class Fog: public ShaderGenerator, public Serializable
{
    ShaderCode* density;
    ShaderCode* color;

public:
    explicit Fog(Reader *reader);
    ~Fog();

    void serialize(Writer *writer) const;
    void makeShaders(const ShaderEmitter &emitter);
};

#endif // FOG_H
