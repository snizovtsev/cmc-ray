#ifndef SCENE_H
#define SCENE_H

#include <QtOpenGL>

#include "interfaces.h"
#include "shadercode.h"
#include "material.h"
#include "item.h"
#include "fog.h"

class Scene: public ShaderGenerator, public Serializable
{
    ShaderCode* light;
    ShaderCode* eye;
    Fog*        fog;

    typedef QHash <QString, Material*> MaterialStorage;
    MaterialStorage  material;
    QList <Item*>    items;
public:
    explicit Scene(Reader* reader);
    ~Scene();

    void serialize(Writer *writer) const;
    void makeShaders(QGLShaderProgram *program);
};

#endif // SCENE_H
