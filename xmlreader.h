#ifndef XMLREADER_H
#define XMLREADER_H

#include "serialization.h"

/**
  @class XMLReader
  Сериализатор чтения файлов в формате XML.
  */
class XMLReader : public Reader
{
    /// Поток парсера XML.
    QXmlStreamReader* xml;
    /// Файл, из которого читаем.
    QFile file;

    /// Хранит сохранённые аттрибуты XML элемента.
    QHash <QString, QString> m_attr;

    /// Пропускает комментарии и текст, пока не найдет открывающий элемент.
    /// @returns true, если открывающий элемент найден, false иначе.
    bool skipUntilStart();
public:
    /// Создаёт сериализатор чтения из файла.
    /// @param fileName - имя файла, из которого читаем.
    XMLReader(const QString& fileName);

    /// Виртуальный деструктор, закрывает поток чтения.
    ~XMLReader();

    QString attrib(const QString &name) const;
    QString text() const;
    QString handleObject();
    void endObject();
    QString child();
};

#endif // XMLREADER_H
