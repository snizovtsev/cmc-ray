#ifndef COLORMODEL_H
#define COLORMODEL_H

#include <QtCore>

#include "serialization.h"
#include "shadergenerator.h"

/**
  @interface ColorModel
  Общий интерфейс для классов, реализующих модели света.
  @warning При сериализации данных классов Вы должны самостоятельно
  вызвать Reader::handleObject(), т.к. имя модели содержится в атрибуте.
  */
class ColorModel: public ShaderGenerator, public Serializable
{
public:
    /**
      Создаёт объект и загружает из файла библиотечный код.
      */
    ColorModel();

    /// Создаёт шейдер модели света.
    /// @param matName - имя материала, для которого создаётся шейдер.
    /// @param emitter - программа, в которую добавляется шейдер.
    virtual void makeShaders(const QString& matName, const ShaderEmitter &emitter) = 0;
    void makeShaders(const ShaderEmitter &emitter);

    /// Фабрика моделей диффузных составляющих цвета.
    /// @param name - имя модели.
    /// @param reader - сериализатор.
    static ColorModel* createDiffuse(const QString& name, Reader *reader);
    /// Фабрика моделей бликовых составляющих цвета.
    /// @param name - имя модели.
    /// @param reader - сериализатор.
    static ColorModel* createSpecular(const QString& name, Reader *reader);
};

#endif // COLORMODEL_H
