#ifndef BINARYREADER_H
#define BINARYREADER_H

#include <QtCore>
#include "serialization.h"

/**
  @class BinaryReader
  Сериализатор чтения файлов в бинарном формате.
  */
class BinaryReader: public Reader
{
    /// Файл, из которого читаем.
    QFile file;
    /// Поток чтения.
    QDataStream stream;
    /// Атрибуты.
    QHash <QString, QString> attr;
    /// Текст объекта.
    QString m_text;
    /// Текущий тип токена.
    int tokenType;
    /// Переходит к обработке следущего токена.
    int nextToken();
    /// Последнее считанное имя объекта.
    QString objectName;
public:
    /// Создаёт сериализатор чтения из файла.
    /// @param fileName - имя файла.
    /// @throws SerializeException если не удаётся открыть файл на чтение.
    BinaryReader(const QString& fileName);

    /// Виртуальный деструктор, закрывает поток чтения.
    ~BinaryReader() { }

    QString attrib(const QString &name) const;
    QString text() const;
    QString handleObject();
    void endObject();
    QString child();
};

#endif // BINARYREADER_H
