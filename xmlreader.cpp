#include "xmlreader.h"

XMLReader::XMLReader(const QString &fileName)
    : file(fileName)
{
    if (!file.open(QFile::ReadOnly))
        throw SerializeException("Can't load file");

    xml = new QXmlStreamReader(&file);
    xml->setNamespaceProcessing(false);
    xml->readNext();
}

XMLReader::~XMLReader()
{
    delete xml;
}

bool XMLReader::skipUntilStart()
{
    while (xml->isCharacters() || xml->isComment() || xml->isStartDocument() || xml->isEndDocument())
        xml->readNext();
    return xml->isStartElement();
}

QString XMLReader::attrib(const QString &name) const
{
    return m_attr.value(name);
}

QString XMLReader::text() const
{
    while (xml->isComment())
        xml->readNext();
    if (xml->isCharacters())
        return xml->text().toString();

    throw SerializeException("XML Characters expected");
}

QString XMLReader::handleObject()
{
    if (!skipUntilStart())
        throw SerializeException("XML StartElement expected");

    QString name = xml->qualifiedName().toString();
    m_attr.clear();
    foreach (QXmlStreamAttribute attr, xml->attributes())
        m_attr[attr.name().toString()] = attr.value().toString();

    xml->readNext();
    //qDebug() << "handleObject(): " << name;
    return name;
}

void XMLReader::endObject()
{
    if (hasChild())
        throw SerializeException(QString("Extra children found: ") + child());

    if (xml->isEndElement()) {
        //qDebug() << "child(): ending " << xml->qualifiedName();
        xml->readNext();
    } else {
        throw SerializeException(QString("Unexpected xml element: ") + xml->tokenString());
    }
}

QString XMLReader::child()
{
    if (skipUntilStart())
        return xml->qualifiedName().toString();
    //qDebug() << "child(): not found";

    return QString::Null();
}
