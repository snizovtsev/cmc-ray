#include "binaryreader.h"

enum TokenType {
    START_ELEMENT,
    END_ELEMENT,
    BODY_ELEMENT,
    TEXT_ELEMENT
};

int BinaryReader::nextToken()
{
    do {
        stream >> tokenType;
        if (tokenType == TEXT_ELEMENT) {
            stream >> m_text;
        } else break;
    } while (true);
    return tokenType;
}

BinaryReader::BinaryReader(const QString &fileName)
    : file(fileName)
{
    if (!file.open(QFile::ReadOnly))
        throw SerializeException("Can't open " + fileName);
    stream.setDevice(&file);

    if (nextToken() != START_ELEMENT)
        throw SerializeException("START_ELEMENT expected");
    stream >> objectName;
    nextToken();
}

QString BinaryReader::attrib(const QString &name) const
{
    return attr.value(name);
}

QString BinaryReader::text() const
{
    return m_text;
}

QString BinaryReader::handleObject()
{
    if (tokenType != BODY_ELEMENT)
        throw SerializeException("BODY_ELEMENT expected");

    int attr_count;
    stream >> attr_count;
    attr.clear();
    for (int i = 0; i < attr_count; ++i) {
        QString name, value;
        stream >> name >> value;
        attr.insert(name, value);
    }
    nextToken();

    QString tmp = objectName;
    objectName.clear();
    return tmp;
}

void BinaryReader::endObject()
{
    if (tokenType != END_ELEMENT)
        throw SerializeException("Unhandled object found");
    nextToken();
}

QString BinaryReader::child()
{
    // Уже прочитали имя, просто отдаём его.
    if (tokenType == BODY_ELEMENT)
        return objectName;

    if (tokenType == START_ELEMENT) {
        stream >> objectName;
        nextToken();
        return objectName;
    }

    // Дочерних элементов больше нет.
    return QString::Null();
}
