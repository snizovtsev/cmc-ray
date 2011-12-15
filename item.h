#ifndef ITEM_H
#define ITEM_H

#include "interfaces.h"
#include "shadercode.h"

class Item: public ShaderGenerator, public Serializable
{
    QString       name;
    ShaderCode*   code;
    QString       material;
    ShaderCode*   ambientOcclusion;
    ShaderCode*   softShadows;
public:
    explicit Item(Reader* reader);
    ~Item();

    void serialize(Writer *writer) const;
    void makeShaders(QGLShaderProgram *program);
};

#endif // ITEM_H
