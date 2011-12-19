#ifndef SHADERCODE_H
#define SHADERCODE_H

#include <QtCore>

#include "serialization.h"

/**
  @class ShaderCode
  Хранит код шейдера или шейдерного выражения.
  */
class ShaderCode : public Serializable
{
    /// Имя объекта. Определяется во время сериализации.
    QString objectName;
    /// Код шейдера или выражения.
    QString code;
    /// Код загловка.
    QString m_require;

    /// Общая часть конструкторов.
    void construct(Reader* reader);
public:
    /// Создаёт класс без сериализации.
    /// @param _objectName - имя объекта.
    /// @param _code - код шейдера.
    explicit ShaderCode(const QString &_objectName, const QString &_code);

    /// Создаёт класс сериализацией.
    /// Имя объекта определяется автоматически.
    /// @param reader - сериализатор чтения.
    explicit ShaderCode(Reader* reader);
    /// Создаёт класс сериализацией и проверяет имя объекта.
    /// @param name - ожидаемое имя объекта.
    /// @param reader - сериализатор чтения.
    /// @throws SerializeException если имя считанного объекта другое.
    ShaderCode(QString name, Reader *reader);

    void serialize(Writer *writer) const;

    /// Оператор преобразования в строку с кодом.
    operator QString() { return code; }
    /// Загловок шейдерного выражения.
    /// Код, который нужно вставить в заголовок шейдера, в котором используется
    /// это выражение. Может содержать, к примеру, описание юниформа.
    QString require() const { return m_require; }
};

#endif // SHADERCODE_H
