#ifndef SCENE_H
#define SCENE_H

#include <QtOpenGL>

#include "serialization.h"
#include "shadergenerator.h"
#include "shadercode.h"
#include "material.h"
#include "item.h"
#include "fog.h"


/**
  @class Scene
  Соединяет вместе материал, объекты и остальное.
  Основная задача - создать все нужние дочерние шейдеры и сгенерировать
  функции цвета и расстояния для сцены целиком, а не для одного объекта на
  ней.
  */
class Scene: public ShaderGenerator, public Serializable
{
    /// Положение источника света.
    /// Шейдерное выражение типа <b>vec3</b>.
    ShaderCode* light;
    /// Начальное положение наблюдателя.
    /// Шейдерное выражение типа <b>vec3</b>.
    ShaderCode* eye;
    /// Туман
    Fog*        fog;

    /// Тип контейнера для хранения материалов.
    typedef QHash <QString, Material*> MaterialStorage;

    /// Все материалы сцены.
    MaterialStorage  material;
    /// Все объекты сцены.
    QList <Item*>    items;
public:
    /// Создаёт сцену сериализацией.
    /// @param reader - сериализатор.
    explicit Scene(Reader* reader);
    /// Виртуальный деструктор.
    ~Scene();

    void serialize(Writer *writer) const;
    void makeShaders(const ShaderEmitter &emitter);
};

#endif // SCENE_H
