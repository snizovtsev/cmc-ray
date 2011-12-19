#ifndef ITEM_H
#define ITEM_H

#include "serialization.h"
#include "shadergenerator.h"
#include "shadercode.h"

/**
  @class Item
  Хранит объект сцены и его параметры.
  */
class Item: public ShaderGenerator, public Serializable
{
    /// Имя объекта.
    QString       m_name;
    /// Код объекта, содержащий шейдерную функцию, с прототипом
    /// <pre>float имя_объекта( vec3 point );</pre>
    /// Используется для определения расстояния от объекта до точки.
    ShaderCode*   code;
    /// Имя материала.
    QString       m_material;
    /// Коефициент, регулирующий количество тени на объекте.
    /// По умолчанию равен 1.0
    ShaderCode*   shadowForce;
public:
    /// Создаёт объект сериализацией.
    /// @param reader - сериализатор.
    explicit Item(Reader* reader);
    /// Виртуальный деструктор.
    ~Item();

    /// Имя объекта.
    QString name()     const { return m_name; }
    /// Имя материала, используемого объектом.
    QString material() const { return m_material; }

    void serialize(Writer *writer) const;
    void makeShaders(const ShaderEmitter &emitter);
};

#endif // ITEM_H
