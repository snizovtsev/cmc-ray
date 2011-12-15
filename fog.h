#ifndef FOG_H
#define FOG_H

#include "interfaces.h"
#include "shadercode.h"

class Fog: public ShaderGenerator, public Serializable
{
    ShaderCode* intensity;
    ShaderCode* color;

public:
    explicit Fog(Reader *reader);
    ~Fog();

    void serialize(Writer *writer) const;
    void makeShaders(QGLShaderProgram *program);
};

#endif // FOG_H
