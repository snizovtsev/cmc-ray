#ifndef BINARYWRITER_H
#define BINARYWRITER_H

#include <QtCore>
#include "serialization.h"

/**
  @class BinaryWriter
  Сериализатор записи в бинарный файл.
  */
class BinaryWriter: public Writer
{
    /// Файл, в который пишем.
    QFile file;
    /// Поток записи.
    QDataStream stream;
    /// Атрибуты.
    QList < QPair<QString, QString> > attrs;
public:
    /// Создаёт сериализатор для записи в файл.
    /// @param fileName - имя файла.
    /// @throws SerializeException если не удаётся открыть файл для записи.
    BinaryWriter(const QString& fileName);

    /// Виртуальный деструктор, закрывает поток записи.
    ~BinaryWriter() { }

    void enterObject(const QString &name);
    void pushAttribute(const QString &name, const QString &value);
    void writeText(const QString &text);
    void leaveObject();
};

#endif // BINARYWRITER_H
