#ifndef ITEM_H
#define ITEM_H

#include "interfaces.h"
#include "shadercode.h"

class Item: public ShaderGenerator, public Serializable
{
    QString       m_name;
    ShaderCode*   code;
    QString       m_material;
    ShaderCode*   ambientOcclusion;
    ShaderCode*   softShadows;
public:
    explicit Item(Reader* reader);
    ~Item();

    QString name()     const { return m_name; }
    QString material() const { return m_material; }

    void serialize(Writer *writer) const;
    void makeShaders(QGLShaderProgram *program);
};

#endif // ITEM_H
