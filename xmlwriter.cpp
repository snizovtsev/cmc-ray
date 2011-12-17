#include "xmlwriter.h"

XMLWriter::XMLWriter(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly))
        throw SerializeException("Can't write to file");

    xml = new QXmlStreamWriter(&file);
}

XMLWriter::~XMLWriter()
{
    delete xml;
}

void XMLWriter::enterObject(const QString &name)
{
    xml->writeStartElement(name);
    xml->writeAttributes(attr);
    attr.clear();
}

void XMLWriter::pushAttribute(const QString &name, const QString &value)
{
    attr.append(name, value);
}

void XMLWriter::writeText(const QString &text)
{
    xml->writeCharacters(text);
}

void XMLWriter::leaveObject()
{
    xml->writeEndElement();
}
