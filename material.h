#ifndef MATERIAL_H
#define MATERIAL_H

#include <QtCore>

#include "serialization.h"
#include "shadergenerator.h"
#include "colormodel.h"
#include "refract.h"

/**
  @class Material
  Хранит компоненты материала и отвечает за их смешивание.
  */
class Material : public ShaderGenerator, public Serializable
{
    /// Диффузная составляющая
    ColorModel* diffuse;
    /// Бликовая составляющая
    ColorModel* specular;
    /// Преломления и отражения
    Refract*    refract;
    /// Имя материала
    QString     m_name;

    /// Генерирует шейдерную программу `colorAt()`.
    /// @param no_refract - генерировать ли вариант без отражений
    ///    (чтобы не было рекурсии)
    void colorAt(const ShaderEmitter &emitter, bool no_refract);
public:
    /// Создаёт объект сериализацией.
    /// @param reader - сериализатор.
    explicit Material(Reader* reader);
    /// Виртуальный деструктор.
    ~Material();

    void serialize(Writer *writer) const;
    void makeShaders(const ShaderEmitter &emitter);

    /// Имя материала.
    QString name() const { return m_name; }
};

#endif // MATERIAL_H
