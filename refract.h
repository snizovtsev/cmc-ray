#ifndef REFRACT_H
#define REFRACT_H

#include "serialization.h"
#include "shadergenerator.h"
#include "shadercode.h"

/**
  @class Refract
  Отвечает за отражение и преломление.
  */
class Refract: public ShaderGenerator, public Serializable
{
    /// Имя материала
    QString m_id;
    /// Множитель для смешивания с основным цветом.
    /// Шейдерное выражение типа <b>float</b>
    ShaderCode* m_factor;
    /// Коэфициент преломления.
    /// Шейдерное выражение типа <b>float</b>
    ShaderCode* indexOfRefraction;

    /// Максимальное число итераций для трассировки отражений.
    int reflectLimit;
    /// Максимальное число итераций для трассировки преломлений.
    int refractLimit;
public:
    /// Создаёт объект сериализацией.
    /// @param id - имя материала.
    /// @param reader - сериализатор.
    explicit Refract(const QString &id, Reader* reader);
    /// Виртуальный деструктор.
    ~Refract();

    ShaderCode* factor() const { return m_factor; }

    void serialize(Writer *writer) const;
    void makeShaders(const ShaderEmitter &emitter);
};

#endif // REFRACT_H
