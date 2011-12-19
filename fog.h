#ifndef FOG_H
#define FOG_H

#include "serialization.h"
#include "shadergenerator.h"
#include "shadercode.h"

/**
  @class Fog
  Создаёт туман. Добавляет шейдер с функцией `applyFog()`
  для создания эффекта тумана.
  */
class Fog: public ShaderGenerator, public Serializable
{
    /// Глубина тумана.
    ShaderCode* density;
    /// Цвет тумана.
    ShaderCode* color;

public:
    /// Создаёт объект сериализацией.
    /// @param reader - сериализатор.
    explicit Fog(Reader *reader);
    /// Виртуальный деструктор.
    ~Fog();

    void serialize(Writer *writer) const;
    void makeShaders(const ShaderEmitter &emitter);
};

#endif // FOG_H
