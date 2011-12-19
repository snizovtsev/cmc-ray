#ifndef XMLWRITER_H
#define XMLWRITER_H

#include <QtCore>

#include "serialization.h"

/**
  @class XMLWriter
  Сериализатор записи в файлы формата XML.
  */
class XMLWriter : public Writer
{
    /// Поток записи.
    QXmlStreamWriter* xml;
    /// Файл, в который пишем.
    QFile file;
    /// Атрибуты.
    QXmlStreamAttributes attr;

public:
    /// Создаёт сериализатор для записи в файл.
    /// @param fileName - имя файла, в который пишем.
    XMLWriter(const QString &fileName);
    /// Виртуальный деструктор, закрывает поток записи.
    ~XMLWriter();

    void enterObject(const QString &name);
    void pushAttribute(const QString &name, const QString &value);
    void writeText(const QString &text);
    void leaveObject();
};

#endif // XMLWRITER_H
