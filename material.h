#ifndef MATERIAL_H
#define MATERIAL_H

#include <QtCore>

#include "interfaces.h"
#include "colormodel.h"

class Material : public ShaderGenerator, public Serializable
{
    ColorModel* diffuse;
    ColorModel* specular;
    QString     m_name;

public:
    explicit Material(Reader* reader);
    ~Material();

    void serialize(Writer *writer) const;
    void makeShaders(QGLShaderProgram *program);

    QString name() const { return m_name; }
};

#endif // MATERIAL_H
